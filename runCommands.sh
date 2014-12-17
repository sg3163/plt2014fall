#!/bin/bash
ocamllex scanner.mll
ocamlyacc parser.mly
ocamlc -c ast.mli
ocamlc -c parser.mli
ocamlc -c scanner.ml
ocamlc -c parser.ml
ocamlc -c sast.mli 
ocamlc -c symboltable.ml
ocamlc -c analyzer.ml
#load Str.cma
ocamlc -c jo.ml
ocamlc -o jo parser.cmo scanner.cmo symboltable.cmo analyzer.cmo Str.cma jo.cmo
