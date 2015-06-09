#! /bin/sh
# make sure you always put $f in double quotes to avoid any nasty surprises i.e. "$f"

for f in $1/*
do
  echo "Processing $f"
  cp $f input.txt
  time ./vertexcover_finale.x
  echo "Result Ready"
  res="${f//input/result}"
  java Verificatore $f $res output.txt
  echo "Verificatore Finish"
done