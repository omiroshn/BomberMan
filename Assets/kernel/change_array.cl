void kernel change_array(global float* A, global float* C) {
			int id = get_global_id(0);
		//	if (time % 1000 == 0)
				A[id] += 0.05;
			C[id] =  A[id];
};
