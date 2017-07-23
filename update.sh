#!/bin/bash

git pull
npm install

sudo supervisorctl restart ledserv
