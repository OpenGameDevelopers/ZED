#!/bin/bash

GITHASH=`git log --pretty=format:"%H" -1`
GITHASHSHORT=`git log --pretty=format:"%h" -1`
GITDATE=`git log --pretty=format:"%cd" -1`
GITROLLINGCOUNT=`git log --pretty=format:'' | wc -l`
GITHEADER=$1
PROJECT=`echo $2 | awk '{print toupper( $0 )}'`
BRANCH=`git rev-parse --abbrev-ref HEAD`

TAG_NAME=`git describe --tags | sed -e 's/_[0-9].*//'`
VERSION_NUM=""
MAJOR_BUILD_NUM=""
MINOR_BUILD_NUM=""
REVISION_BUILD_NUM=""
BUILD_NUM=""
if [[ $TAG_NAME != "" ]]
then
	VERSION_NUM=`git describe --match "${TAG_NAME}_[0-9]*" HEAD | sed -e 's/-g.*//' -e "s/${TAG_NAME}_//"`
	MAJOR_BUILD_NUM=`echo $VERSION_NUM | sed 's/-[^.]*$//' | sed -r 's/.[^.]*$//' | sed -r 's/.[^.]*$//'`
	MINOR_BUILD_NUM=`echo $VERSION_NUM | sed 's/-[^.]*$//' | sed -r 's/.[^.]*$//' | sed -r 's/.[.]*//'`
	REVISION_BUILD_NUM=`echo $VERSION_NUM | sed 's/-[^.]*$//' | sed -r 's/.*(.[0-9].)//'`
	BUILD_NUM=`echo $VERSION_NUM | sed -e 's/[0-9].[0-9].[0-9]//' -e 's/-//'`
fi

printf "Generating $2 Git Version Header...\n"
printf "Tag name: $TAG_NAME\n"
printf "Revision: $ZEDROLLINGCOUNT [$GITHASH] $GITDATE\n"

printf "#ifndef __%s_SYSTEM_GITVERSION_HPP__\n" $PROJECT > $GITHEADER
printf "#define __%s_SYSTEM_GITVERSION_HPP__\n\n" $PROJECT >> $GITHEADER

printf "#define ZED_VERSION_COMMITHASH			\"%s\"\n" "$GITHASH" >> $GITHEADER
printf "#define ZED_VERSION_COMMITHASHSHORT		\"%s\"\n" "$GITHASHSHORT" >> $GITHEADER
printf "#define ZED_VERSION_COMMITTERDATE		\"%s\"\n" "$GITDATE" >> $GITHEADER
printf "#define ZED_VERSION_ROLLINGCOUNT		%d\n\n" $GITROLLINGCOUNT >> $GITHEADER

if [[ $TAG_NAME != "" ]]
then
	printf "#define ZED_VERSION_MAJOR_BUILD_VERSION		%d\n" $MAJOR_BUILD_NUM >> $GITHEADER
	printf "#define ZED_VERSION_MINOR_BUILD_VERSION		%d\n" $MINOR_BUILD_NUM >> $GITHEADER
	printf "#define ZED_VERSION_REVISION_BUILD_NUM		%d\n" $REVISION_BUILD_NUM >> $GITHEADER
	printf "#define ZED_VERSION_BUILD_NUM				%d\n" $BUILD_NUM >> $GITHEADER
	printf "#define ZED_VERSION_BUILD_STRING			\"%d.%d.%d.%d\"\n" $MAJOR_BUILD_NUM $MINOR_BUILD_NUM $REVISION_BUILD_NUM $BUILD_NUM >> $GITHEADER
	printf "#define ZED_VERSION_TAG						\"%s\"\n" $TAG_NAME >> $GITHEADER
	printf "#define ZED_VERSION_BRANCH					\"%s\"\n\n" $BRANCH >> $GITHEADER
fi
printf "#endif // __ZED_SYSTEM_GITVERSION_HPP__\n\n" >> $GITHEADER

printf "Done\n"
