#!/bin/sh


#if [ ! -f "c/libraries/liblist.a" ] || [ ! -f "c/libraries/libpath.a" ] ; then
#    cd c/libraries
#    make >> lib_msgs.txt
#    cd ../..
#fi
#
if [ ! -f "./preprocessor" ]; then
    make -f MakePreProc >> make.log
fi

if [ ! -f "./jo" ]; then
   make -f Makefile >> make.log
fi

# jo exectutable
JO="./jo"

# preprocessor executable
PRE="./preprocessor"

# Error directory
#ERR=/testfiles/errors

# Compare <outfile> <reffile> <difffile>
# Compares the outfile with reffile.  Differences, if any, written to difffile
Compare() {
	difference=$(diff -b $1 $2)
	echo $difference
	if [ "$difference" != "" ]; then
		echo $difference > $3
	fi
}

function compileAndRun() {
	basename=`echo $1 | sed 's/.*\\///
                            s/.jo//'`
    #echo $basename
	reffile=`echo $1 | sed 's/.jo$//'`
    prepfile=$basename'.pjo'
    #echo $prepfile
    basedir="`echo $1 | sed 's/\/[^\/]*$//'`/"

    # gets the path of the test output file
	testoutput=`echo ${basedir}test_outputs/$basename.c.out`

    echo "Preprocessing '$1'"
	$PRE $1 $prepfile && echo "Preprocessor for $1 succeeded"

	echo "Compiling '$prepfile'"
	if [ ! -f $prepfile ]; then
		echo "$prepfile does not exist"
       return
	fi
	# converting from JO to C++
    $JO $prepfile > "${reffile}.cpp" && echo "Ocaml to C++ of $1 succeeded"



    # compliling the C++ file
    if [ -f "${reffile}.cpp" ]; then
 #   	gcc -Ic/libraries -Lc/libraries -llist -lpath -w -o "${reffile}" "${reffile}.c" 2>> errors.txt
 		g++ "${reffile}.cpp" -o "${reffile}.out" 2>> error.txt
    else
    	echo "Compiling $1 failed"
    	return
    fi

 	# running the binary
    if [ -f "${reffile}.out" ]; then
        eval ${reffile}.out >> ${reffile}.output
        eval ${reffile}.out 
        echo "\n"
        Compare ${reffile}.output ${reffile}.exp ${reffile}.error

#        rm -rf ${reffile}.fdlp
#        rm -rf ${reffile}.cpp
        rm -rf ${reffile}.out
        rm -rf ${reffile}.output
        echo "ran $1 successfully"
    else
        echo "C++ to binary of ${reffile}.cpp failed"
    fi

}

files=testfiles/*.jo

for file in $files
do
	compileAndRun $file
done

#if [ -f $1 ]; then
#	compileAndRun $1
#else
#	echo "$1 doesnt exist"
#fi
