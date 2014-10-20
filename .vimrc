set nu
"set list 
set nolist
set nomagic
set autoindent
set updatetime=1000
set mouse=c
set ignorecase 
let Tlist_Show_One_File=1
let Tlist_Use_Right_Window=1
map <F3> :NERDTreeToggle<CR>
map <F4> :TlistToggle<CR>
map <F5> :set mouse=a<CR>
map <F6> :set mouse=c<CR>
map <F8> :!search <cword> "*" .<CR>
map <F9> :!make<CR>
map <F10> :cs find c <cword><CR>
map <F2> :cs find s <cword><CR>
map <F12> :cs find f <cword><CR>

map <CR>  <C-]> 
map <BS>  <C-T> 
"map <C-F10>  :bn<CR>
"map <C-F11>  :bp<CR> 
" Let NERDTree auto start
autocmd VimEnter * NERDTree
" Let the cursor not in NERDTree window when start
autocmd VimEnter * wincmd p
set expandtab
" set tab as 4 spaces
set shiftwidth=4
set tabstop=4
set softtabstop=4
set guitablabel=7
set whichwrap+=h,l 
set cindent
set backspace=eol,start,indent
"set cursorline
syntax enable
syntax on


set hlsearch
"Set mapleader
let mapleader = ","
let g:mapleader = ","
"×¢ÊÍ
nmap <silent> <F7> ,x
vmap <silent> <F7> ,x
imap <silent> <F7> ,x

"auto add up
filetype plugin indent on
set completeopt=longest,menu
set paste


map ,- :s/^/--/<CR>
map ,# :s/^/#/<CR>
map ,/ :s/^/\/\//<CR>

if has("cscope")
    set csprg=/usr/bin/cscope        " 指定用来执行cscope的命令
    set csto=0                        " 设置cstag命令查找次序：0先找cscope数据库再找标签文件；1先找标签文件再找cscope数据库
    set cst                            " 同时搜索cscope数据库和标签文件
"    set cscopequickfix=s-,c-,d-,i-,t-,e-    " 使用QuickFix窗口来显示cscope查找结果
    set nocsverb
    if filereadable("cscope.out")    " 若当前目录下存在cscope数据库，添加该数据库到vim
        cs add cscope.out
    elseif $CSCOPE_DB != ""            " 否则只要环境变量CSCOPE_DB不为空，则添加其指定的数据库到vim
        cs add $CSCOPE_DB
    endif
    set csverb
endif





