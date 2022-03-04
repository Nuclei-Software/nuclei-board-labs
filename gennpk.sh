#!/bin/env bash

FORCE=false

HOMEPAGE="https://github.com/Nuclei-Software/nuclei-board-labs/tree/master"

if [ "x$1" == "x-f" ] ; then
    FORCE=true
    echo "Will overwrite existing npk.yml if found!"
fi

for board in e203_hbirdv2 rvstar ;
do
    appmks=($(find $board -type f -name "Makefile"))
    for appmk in ${appmks[@]};
    do
        appdir=$(dirname $appmk)
        # https://stackoverflow.com/questions/5928156/replace-one-character-with-another-in-bash
        appname=${appdir//\//_}
        appdesc="${appdir//\// }"
        appflags=""
        apphomepage="$HOMEPAGE/$appdir"
        match=$(grep "COMMON_FLAGS" $appmk)
        if [[ $? == 0 ]] ; then
            appflags="$(echo $match | cut -d "=" -f2)"
            # strip whitespace in appflags
            # https://stackoverflow.com/a/12973694
            appflags=" $(echo $appflags | xargs)"
            echo "Found $match in $appmk"
        fi
        if [ "$board" == "e203_hbirdv2" ] ; then
            if [[ "$appdir" == *"/common/"*  ]] ; then
                appdep="ssp-hsdk_hbirdv2"
            elif [[ "$appdir" == *"/ddr200t/"* ]] ; then
                appdep="bsp-hsdk_ddr200t"
            elif [[ "$appdir" == *"/mcu200t/"* ]] ; then
                appdep="bsp-hsdk_mcu200t"
            fi
        elif [ "$board" == "rvstar" ] ; then
            appdep="bsp-nsdk_gd32vf103v_rvstar"
        else
            continue
        fi
        appnpkyml=$appdir/npk.yml
        if [ ! -f $appnpkyml ] ||  $FORCE ; then
            cp -f npk.yml.tpl $appnpkyml
            sed -i 's|`appname`|'"$appname"'|g'  $appnpkyml
            sed -i 's|`appdesc`|'"$appdesc"'|g'  $appnpkyml
            sed -i 's|`appdep`|'"$appdep"'|g'  $appnpkyml
            sed -i 's|`appflags`|'"$appflags"'|g'  $appnpkyml
            sed -i 's|`apphomepage`|'"$apphomepage"'|g'  $appnpkyml
            echo "Generate $appnpkyml"
        fi
    done
done
