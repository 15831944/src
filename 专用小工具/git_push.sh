#!/bin/bash

function GetRandom(){
	min=10;
    max=20;
    num=$(date +%s+%N);
    ((retnum=num%max+min));
    return $retnum;
}

function PushTenTimes()
{
	cd "F:\\22. LIFE GAME\\myblog\\chrishuppor.github.io"
	GetRandom
	PushTime=$?
	echo "will push $PushTime times"
	for ((i=1;i<=$PushTime;i++))
	do
		fullpath="F:\\22. LIFE GAME\\myblog\\chrishuppor.github.io\\_drafts\\helloworld.txt"
		echo $i >> "$fullpath"
		git add .
		git commit -m "update"
		git push origin master
	done

	rm "$fullpath"
}

function PushRepo()
{
	#commit
	if [[ $1 != '' ]]
	then
		commitMes=$2
	else
		commitMes='update'
	fi

	git add .
	git commit -m $commitMes
	git push origin master
}


#repo
if [[ $1 = 'blog' ]];
then
	cd "F:\\22. LIFE GAME\\myblog\\chrishuppor.github.io"
	PushRepo $2
elif [[ $1 = 'src' ]];
then
	cd "F:\\22. LIFE GAME\\myblog\\src"
	PushRepo $2

#only push
elif [[ $1 = 'OnlyPush' ]];
then
	PushTenTimes

#usage
else
	echo "usage1 to push local repo: <repotype-'blog' or 'src'> (<commit>)"
	echo "usage2 to contribute 10 times: <'OnlyPush'>"
fi
exit