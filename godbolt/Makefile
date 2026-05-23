M = ./manage.sh

reprovision:
	$(M) stop
	$(M) distresetup
	$(M) start -m 8G
	$(M) wait || :
	$(MAKE) provision
	$(MAKE) reinstall_yio
	$(MAKE) config_ce
	$(M) stop
	$(M) start
	$(M) wait
provision:
	$(M) ansible ansible_provision_compiler_explorer.yml
config_ce:
	$(M) ansible ansible_config_ce.yml
reinstall_yio:
	$(M) ansible ansible_reinstall_yio.yml

