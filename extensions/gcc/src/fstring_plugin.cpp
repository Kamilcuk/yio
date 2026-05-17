#include "fstring_ast.hpp"
#include "fstring_gimple.hpp"

int plugin_is_GPL_compatible;

bool warn_fstring_format = true;
bool error_fstring_format = false;
bool fstring_optimizations = false;

static struct attribute_spec fstring_format_attr = {
    "fstring_format", 1, 1, false, false, false, false, NULL, NULL};

static void register_attributes(void *event_data, void *data) {
  register_attribute(&fstring_format_attr);
}

static void plugin_pre_genericize(void *event_data, void *data) {
  tree fndecl = (tree)event_data;
  walk_tree_without_duplicates(&DECL_SAVED_TREE(fndecl), walk_tree_callback, fndecl);
}

int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version) {
  if (!plugin_default_version_check(version, &gcc_version)) { return 1; }

  bool opt_enabled = true;
  for (int i = 0; i < plugin_info->argc; i++) {
    if (strcmp(plugin_info->argv[i].key, "Wfstring-format") == 0) {
      if (plugin_info->argv[i].value) { warn_fstring_format = atoi(plugin_info->argv[i].value); }
    } else if (strcmp(plugin_info->argv[i].key, "Werror-fstring-format") == 0) {
      if (plugin_info->argv[i].value) { error_fstring_format = atoi(plugin_info->argv[i].value); }
    } else if (strcmp(plugin_info->argv[i].key, "ffstring-optimizations") == 0) {
      if (plugin_info->argv[i].value) { opt_enabled = atoi(plugin_info->argv[i].value); }
      else { opt_enabled = true; }
    }
  }

  fstring_optimizations = opt_enabled && (optimize >= 1);

  register_callback(plugin_info->base_name, PLUGIN_ATTRIBUTES, register_attributes, NULL);
  register_callback(plugin_info->base_name, PLUGIN_PRE_GENERICIZE, plugin_pre_genericize, NULL);
  
  struct register_pass_info pass_fold;
  pass_fold.pass = new pass_fstring_fold(g);
  pass_fold.reference_pass_name = "ssa";
  pass_fold.ref_pass_instance_number = 1;
  pass_fold.pos_op = PASS_POS_INSERT_AFTER;
  register_callback(plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_fold);

  struct register_pass_info pass_merge;
  pass_merge.pass = new pass_fstring_merge(g);
  pass_merge.reference_pass_name = "optimized";
  pass_merge.ref_pass_instance_number = 1;
  pass_merge.pos_op = PASS_POS_INSERT_BEFORE;
  register_callback(plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_merge);

  return 0;
}
