
au BufRead *.m4 set filetype=c
set makeprg=make\ build
command! -bang -nargs=* Ag call fzf#vim#ag(<q-args>, "--follow -U --ignore _build --ignore .git", fzf#vim#with_preview(), <bang>0)
autocmd FileType cmake setlocal autoindent noexpandtab tabstop=4 shiftwidth=4

