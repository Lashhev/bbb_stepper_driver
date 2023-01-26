#!/bin/bash
if [ "$#" -ne 2 ]; then
    echo "usage: build.sh SSH_PUBLIC_KEY SSH_PRIVATE_KEY"
    echo "Don't worry your SSH keys can't be extracted from the building image. The keys are neccessary to clone some git repositories from GITHUB"
    exit 1
fi
SSH_PUBLIC_KEY=$1
SSH_PRIVATE_KEY=$2
echo "${SSH_PRIVATE_KEY}"
echo "Building main docker image with the BBB GPIO libraries"
docker build . --rm --build-arg SSH_PRIVATE_KEY="$(cat ${SSH_PRIVATE_KEY})" --build-arg SSH_PUBLIC_KEY="$(cat ${SSH_PUBLIC_KEY})" --platform linux/arm64/v8 --tag arm64v8/ubuntu/dip_stepper:1.0.0 