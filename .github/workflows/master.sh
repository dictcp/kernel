set -eu
cd kernel
cp ../dotconfig .config
# the cloudready config expects keys at /keys/... which does not exist here
echo 'CONFIG_MODULE_SIG_KEY="certs/signing_key.pem"' >> .config
make olddefconfig
make CC="ccache gcc" KCFLAGS="-Werror -Wno-packed-not-aligned -Wno-tautological-compare -Wno-maybe-uninitialized" -j16
cd ~
zip -r dotccache.zip .ccache
cp dotccache.zip /home/kewluser/app/kernel/artifacts
