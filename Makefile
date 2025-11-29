
auto_formater:
	pre-commit run --all-files -c .pre-commit-config.yaml

benchmark_start:
	./build/benchmark/yolo --benchmark_format=console --benchmark_out=file

test:
	ctest -C Release --output-on-failure -T test --test-dir build