TEMPLATE = subdirs

SUBDIRS += rmscrypto_sdk rmsauth_sdk

# 添加 rms_sdk 作为自定义目标
system(cmake -S rms_sdk -B rms_sdk/build)
system(cmake --build rms_sdk/build)

rmsauth_sdk.depends = rmscrypto_sdk