log.log: ./exe.exe src.src
	./exe.exe < src.src > log.log && tail $(TAIL) log.log