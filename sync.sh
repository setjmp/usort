#!/bin/sh -e

#synchronize emacs repo with xcalibur (known as setjmp.com across the interweb)
HG_USER=ais
HG_HOST=setjmp.com
LOC=/home/ais/src/sorting
URL=ssh://${HG_USER}@${HG_HOST}
echo "pushing to:" $URL
hg com
hg pull --remotecmd  /tools/bin/hg -u $URL/$LOC
hg push --remotecmd  /tools/bin/hg $URL/$LOC

