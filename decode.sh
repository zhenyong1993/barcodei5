#!/bin/bash

sed -n -r 's@#define KEY_([a-zA-Z]*)\s+([0-9xa-f]*).*@case \2: return "\1";@gp' input-event-codes.h
