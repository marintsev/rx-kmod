#!/bin/bash

if grep main /proc/modules; then
	echo Yes
	exit 1
else
	echo No
	exit 0
fi