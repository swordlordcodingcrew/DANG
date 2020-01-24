# DANG as a git submodule

## initial
- Create your directory
- As example add a lib/ directory
- Add everything to your own git, push
- Run this command:
``` shell
git submodule add https://github.com/swordlordcodingcrew/DANG.git dang
```
- Push your directory again

## pulling everything
- git clone --recursive yourproject_repo

## update (warning override!)
- git submodule foreach git pull origin master

## update (merge)
- git submodule update --recursive

to add in in a hook:
``` shell
cd ~/yourproject
echo "git submodule update --init --recursive" >> .git/hooks/post-merge
```