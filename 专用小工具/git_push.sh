#!/bin/bash

if [[ $1 = 'blog' ]];
then
	cd "F:\\22. LIFE GAME\\myblog\\chrishuppor.github.io"
elif [[ $1 = 'src' ]];
then
	cd "F:\\22. LIFE GAME\\myblog\\src"
else
	echo "usage: <repotype-'blog' or 'src'> (<commit>)"
	exit
fi

commitMes='update'
if [[ $2 != '' ]];
then
	$commitMes = $2
fi

git add .
git commit -m '$commitMes'
git push origin master