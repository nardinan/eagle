#!/bin/sh

#
# Copyright (c) 2009, 2012, Oracle and/or its affiliates. All rights reserved.
#

if [ "$1" -eq "64" ]; then
    suffix_64="_64"
else
    suffix_64=
fi

osname=`uname -s`
uname_p=`uname -p`
if [ "${uname_p}" = "unknown" ]; then
    uname_p=`uname -m`
fi

case "${uname_p}" in
    --i86pc|i386|i686|x86_64)
        platform_name="${osname}-x86"
        ;;
    --sparc)
        platform_name="${osname}-sparc"
        ;;
    *)
        echo "Unknown platform: ${uname_p}"
        exit 4
        ;;
esac

platform_name="${platform_name}${suffix_64}"

#echo platform_name: ${platform_name}

#base_dir="`pwd`/../release/bin"
base_dir="`pwd`"
platform_dir=${base_dir}/${platform_name}

output=`mktemp`

if [ ! -x ${platform_dir}/rfs_test_env ]; then
    chmod u+x ${platform_dir}/rfs_test_env
fi

LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${platform_dir} \
    LD_PRELOAD=rfs_preload.so \
    RFS_TEST_ENV=1 \
    ${platform_dir}/rfs_test_env > ${output}

#cat ${output}

gold=`mktemp`
fact=`mktemp`

grep "RFS_TEST_PRELOAD" ${output} | awk '{print $2}' > ${fact}
grep "RFS_TEST_CLIENT" ${output} | awk '{print $2}' > ${gold}

while read func_name; do
    real_func_name=`echo "${func_name}" | awk -F/ '{ print $NF }'`
    grep "${func_name}" ${fact} > /dev/null
    rc=$?
    if [ $rc = 0 ]; then
        echo "${real_func_name} OK"
    else
        echo "${real_func_name} FAILED"
    fi
done < ${gold}

rm ${output}
rm ${gold}
rm ${fact}
