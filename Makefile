sem: cs
	gcc -o semaphore semaphores.c error_handling.c random_gen.c -lpthread

cond: cc
	gcc -o condition condition_variables.c error_handling.c random_gen.c -lpthread

cs:
	@rm semaphore || true

cc:
	@rm condition || true

run_sem: cs sem
	./semaphore $(n)

run_cond: cc cond
	./condition $(n)