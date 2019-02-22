void kernel init_array(global float* A, global float* C) {
			int id = get_global_id(0);
			//A[id] = 0.5;
			if (id < 4) {
				A[id] = id % 2;
			} else if (id < 8) {
				A[id] = (id % 2 ) ? 0 : 1;
			} else if (id < 12) {
				A[id] = 0;
			} else if (id < 16) {
				A[id] = id % 2;
			} else if (id < 20) {
				A[id] = 1;
			} else if (id < 24) {
				A[id] = (id % 2 ) ? 0 : 1;
			}
			C[id] = A[id];
};
