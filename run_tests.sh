for f in `ls bin/tests_bin`
do
	echo "Running $f test..."
	./bin/tests_bin/$f
done

