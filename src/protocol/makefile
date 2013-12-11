all:
	protoc -o slim.pb slim.proto
	python nanopb/generator/nanopb_generator.py slim.pb
	protoc --python_out=../gui slim.proto
