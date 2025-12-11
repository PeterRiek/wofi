#!/bin/sh
../build/wofi \
	--external-run-label ./label.sh \
	--external-run-exec	./exec.sh \
	--show drun
