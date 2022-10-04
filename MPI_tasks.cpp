#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>


// �������� ��������� ��� ������ ����� �������� ���� �������������, ���������� ������� ����� � ������� �Hello World�. ��������� ��������� � 8 ��������. 
// ������ �� ����� ���������? ������?
void task_1() {
	#pragma omp parallel num_threads(8)
	printf("Hello World! Thread ID: %d, threads: %d\n", omp_get_thread_num(), omp_get_num_threads());
}



// �������� ���������, � ������� ���������� ��� ������������ �������, ���������� ������� ������� �� ��������� ��������� #pragma omp parallel if(�), 
// ���� �������� �������� ����� ����� ������ 2, ������� ����������� �����������, ����� �� �����������. ����� ����� ����� ������ �������� ������ ������ 3, 
// ����� ������ � ������ 2. ������ ������������ �������� ���������� ���������� ����� � ����� ������ ����, ��������� ������ �� �����. 
// ��������� � ������������ ������ ���������. 
void task_2() {
	int threads_num = 3;

	#pragma omp parallel num_threads(threads_num) if(threads_num > 2)
	{
		int thread_id = omp_get_thread_num();
		int threads_count = omp_get_num_threads();
		printf("Count of threads: %d, thread id: %d \n", threads_count, thread_id);
	}

	threads_num = 2;

	#pragma omp parallel num_threads(threads_num) if(threads_num > 2)
	{
		int thread_id = omp_get_thread_num();
		int threads_count = omp_get_num_threads();
		printf("Count of threads: %d, thread id: %d \n", threads_count, thread_id);
	}
}

int main() {
	task_2();
}


// �������� ���������, � ������� �������� � ��������� ��������� �������� ������������� ����������: a � b, ���������� ��� ������������ �������, 
// ������ � � ������ ����� 2, ������ � � ������ ����� 4. ��� ������ ������� ���������� a � b �������� �����������, � ������� ������� private � 
// firstprivate ��������������, ������ ������� ���������� ����� ���� � ��������� ���������� �������� ���������� �� ��� ��������. ������ �� ����� 
// �������� ���������� �� ����� � ������������ �������, ������ ������������ ������� � ����� ������ �� ������������ �������. ��� ������ ������� 
// ���������� a �������� �����, ���������� b �������� ����������, � ������� ������� private, ���������� ����� ���� � ��������� ���������� �������� 
// ���������� �� ��� ��������. ������ �� ����� �������� ���������� �� ����� � ������������ �������, ������ ������������ ������� � ����� ������ �� 
// ������������ �������. �������������������� ���������� ����������������.
void task_3() {
	int a = 0, b = 0;

	printf("Before a:%d, b:%d\n", a, b);

	#pragma omp parallel num_threads(2) private(a) firstprivate(b)
	{
		a = 0;
		a += omp_get_thread_num();
		b += omp_get_thread_num();
		printf("BEFORE: thread num:%d, a:%d, b:%d\n", omp_get_thread_num(), a, b);
	}
	printf("AFTER: a:%d, b:%d\n\n", a, b);

	#pragma omp parallel num_threads(4) shared(a) private(b)
	{
		b = 0;
		#pragma omp atomic
		a -= omp_get_thread_num();
		b -= omp_get_thread_num();
		printf("BEFORE: thread num:%d, a:%d, b:%d\n", omp_get_thread_num(), a, b);
	}
	printf("AFTER: a:%d, b:%d\n\n", a, b);
}


// �������� ���������, � ������� �������� � ��������� ��������� �������� ������������� �������� a[10] � b[10], ���������� ������������ �������, 
// ���������� ����� ������ ������ 2, �������� ��� ��� �������� (����� 0) � ���� � ������� 1. �������� ���� (master) ������ ��������� ����� min 
// �������� ��������� ������� a, ���� � ������� 1 - ����� max �������� ��������� ������� b. ��������� ������ �� �����.
void task_4() {
	int n = 10;
	int* a = new int[n];
	int* b = new int[n];

	for (int i = 0; i < n; i++) {
		a[i] = rand();
		b[i] = rand();
		printf("a[%d] = %d, b[%d] = %d \n", i, a[i], i, b[i]);
	}

	int max = INT_MIN;
	int min = INT_MAX;

	#pragma omp parallel num_threads(2)
	{
		int thread_id = omp_get_thread_num();
		if (thread_id == 0) {
			for (int i = 0; i < n; i++) {
				if (min > a[i]) {
					min = a[i];
				}
			}
		} else if (thread_id == 1) {
			for (int i = 0; i < n; i++) {
				if (max < b[i]) {
					max = b[i];
				}
			}
		}
	}
	printf("Min in a:%d, Max in b:%d\n\n", min, max);
}


// �������� ���������, � ������� �������� � ��������� ��������� �������� ��������� ���������� ������� d[6][8], ��� ������������� �������� 
// ������������ ��������� ��������� �����. ��������� ����������� ��������� sections�section ���������� ��� ������ ��� ���������� ��������� ��������: 
// ������ ������ ��������� ���������� �������� ��������������� �������� ��������� ���������� �������,
// ������ ������ ��������� ���������� ������������ � ������������� �������� ��������� ���������� �������,
// ������ ������ ��������� ���������� ���������� ��������� �������, �������� �������� ������� ������ 3.
// � ������ ������ ���������� � ������ �� ����� ����� ����������� ���� � ��������� ���������� ����������.
void task_5() {
	int d[6][8];
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 8; j++) {
			d[i][j] = rand();
		}
	}

	#pragma omp parallel sections shared(d) num_threads(3)
	{
		#pragma omp section
		{
			int sum = 0;
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 8; j++) {
					sum += d[i][j];
				}
			}
			printf("Thread num: %d; Average = %d\n", omp_get_thread_num(), sum / 48);
		}

		#pragma omp section 
		{
			int min = INT_MAX;
			int max = INT_MIN;
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 8; j++) {
					if (min > d[i][j]) {
						min = d[i][j];
					}

					if (max < d[i][j]) {
						max = d[i][j];
					}
				}
			}
			printf("Thread num: %d; max = %d; min = %d\n", omp_get_thread_num(), max, min);
		}

		#pragma omp section 
		{
			int res = 0;
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 8; j++)	{
					if (d[i][j] % 3 == 0) {
						res += 1;
					}
				}
			}
			printf("Thread num: %d; Number of multiples 3: %d\n", omp_get_thread_num(), res);
		}
	}
}


// �������� ���������, � ������� �������� � ��������� ��������� �������� �������������� ������� a[100]. ��������� ����������� parallel for � 
// reduction ��������� ������� �������������� �������� ��������� ������� a. ���� ��� ������ reduction, ������ ���. �������� ���������� ��������. 
// ��������� ��������� �������� �����������. 
void task_6() {
	int a[10] = { 1, 2, 3, 4, 5, 6, 5, 4, 3, 2 };
	int b[10] = { 10, 2, 3, 4, 5, 1, 5, 4, 3, 2 };

	int sum_a = 0, sum_b = 0;
	#pragma omp parallel for reduction(+: sum_a) num_threads(5)
	for (int i = 0; i < 100; i++) {
		sum_a += a[i];
	}
	printf("Average with reduction: %d\n", sum_a / 100);

	#pragma omp parallel num_threads(5)
	{
		#pragma omp parallel for
		for (int i = 0; i < 100; i++) {
			sum_b += b[i];
		}
	}
	printf("Average without reduction: %d\n", sum_b / 100);
}



// �������� ���������, � ������� ���������� ��� ������������ �������, ������ �� ������� �������� ������������ ����������� for ����������� 
// ������������� ��������� ���������� �������� ����� ����� a[12], b[12] � c[12]. ����� ����� ����� ������ �������� ������ ������ 3, 
// ����� ������ � ������ 4. ������ ������������ ������� ��������� ������������� ��������� �������� a � b � �������������� ������������ 
// ������������� ��������, ������ ������ �������� ������� ��������������, ������ ������������ ������� ��������� ������������� ��������� 
// ������� c �� ���������� ������� c[i] = a[i] + b[i], � �������������� ������������� ������������� ��������, ������ ������ �������� �������
// ��������������. � ������ ������� ���������� � ������ �� ����� ���������� �����, ����� ���� � ��������� ���������� �����. ��������� � 
// ������������ ������ ���������.
void task_7() {
	int a[12], b[12], c[12];

	#pragma omp parallel num_threads(3)
	{
		printf("Current thread = %d, threads count = %d.\n", omp_get_thread_num(), omp_get_num_threads());
		#pragma omp for schedule(static)
		for (int i = 0; i < 12; i++) {
			a[i] = rand();
			printf("A[%d] = %d\n", i, a[i]);
			b[i] = rand();
			printf("B[%d] = %d\n", i, b[i]);
		}
	}

	#pragma omp parallel num_threads(4)
	{
		printf("Current thread = %d, threads count = %d.\n", omp_get_thread_num(), omp_get_num_threads());
		#pragma omp for schedule(dynamic)
		for (int i = 0; i < 12; i++) {
			c[i] = a[i] + b[i];
			printf("C[%d] = %d\n", i, c[i]);
		}
	}
}


// �������� ���������, � ������� �������� ������ �� 16000 ��������� � ��������������� ���, ��� �������� �������� ������� ����� ��� 
// ����������� ������. ����� �������� �������������� ������, � ������� (�� ����������� ������� ���������) ����� ������� �������� ��������� �������: 
// b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0
// ��������� ��������� � 8 - � ���������� ��� ��������� ����� ������������� �����(static, dynamic, guided, auto(runtime, ���� 
// auto �� ��������)) � ���������� ����� �� ������ �������� ������.
void task_8() {
	const int N = 16000;
	int a[N];
	int b[N];

	for (int i = 0; i < N; i++) {
		a[i] = i;
	}

	double start = omp_get_wtime();
	#pragma omp parallel num_threads(8)
	{
		#pragma omp for schedule(static)
		for (int i = 1; i < N - 1; i++) {
			b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
		}
	}
	double end = omp_get_wtime();
	printf("Time with static %f\n", end - start);

	start = omp_get_wtime();
	#pragma omp parallel num_threads(8)
	{
		#pragma omp for schedule(dynamic)
		for (int i = 1; i < N - 1; i++) {
			b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
		}
	}
	end = omp_get_wtime();
	printf("Time with dynamic %f\n", end - start);

	start = omp_get_wtime();
	#pragma omp parallel num_threads(8)
	{
		#pragma omp for schedule(guided)
		for (int i = 1; i < N - 1; i++)	{
			b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
		}
	}
	end = omp_get_wtime();
	printf("Time with guided %f\n", end - start);

	start = omp_get_wtime();
	#pragma omp parallel num_threads(8)
	{
		#pragma omp for schedule(auto)
		for (int i = 1; i < N - 1; i++)	{
			b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
		}
	}
	end = omp_get_wtime();
	printf("Time with auto %f\n", end - start);
}



// ��������� ����������� OpenMP �������� ��������� ��������� ������� �� ������. �������� ����� ���������� ���������������� 
// � ������������ �������� (������� ��������� schedule). ���������� ������� ������� ��� ������� ������������ ��������� 
// �������� �������� ������� ���������������� 
void task_9(int N) {
	
}


// �������� ���������, � ������� �������� � ��������� ��������� �������� ��������� ���������� ������� d[6][8], 
// ��� ������������� �������� ������������ ��������� ��������� �����. ��������� ����������� ��������� omp parallel for � omp critical 
// ���������� ����������� � ������������ �������� ��������� ���������� �������. ����������� ������ reduction 
// ��� max � min. ���������� ����� ������ ��������������. ��������� ������ �� �����.
void task_10() {

	int d[6][8];
	int max = INT_MIN;
	int min = INT_MAX;

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 8; j++) {
			d[i][j] = rand();
			printf("%d\n", d[i][j]);
		}
	}

	#pragma omp parallel for num_threads(4)
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 8; j++) {
			if (d[i][j] > max)
			#pragma omp critical
			{
				max = d[i][j];
			}

			if (d[i][j] < min)
			#pragma omp critical
			{
				min = d[i][j];
			}

		}
	}
	printf("Max: %d, Min: %d", max, min);
}


// �������� ���������, � ������� �������� � ��������� ��������� �������� ������� ����� ����� a[30], ��� 
// ������������� �������� ������������ ��������� ��������� �����. ��������� ����������� omp parallel for � omp atomic 
// ��������� ���������� ��������� �������, �������� �������� ������� ������ 9. ���������� ����� ������ ��������������. 
// ��������� ������ �� �����. 
void task_11() {

	int a[30];
	for (int i = 0; i < 30; i++) {
		a[i] = rand() % 1000;
		printf("%d ", a[i]);
	}
	printf("\n");

	int res = 0;

	#pragma omp parallel for num_threads(4)
	for (int i = 0; i < 30; i++) {
		if (a[i] % 9 == 0) {
			#pragma omp atomic
			res++;
		}
	}
	printf("Result: %d", res);
}


// �������� ���������, � �������, �������� � ��������� ���������� ���������� ������ ����� �����. 
// ��������� ����������� OpenMP ����� ������������ �������� ��������� ������� ������� 7. ����� 
// ������� � ���������� ������� ���������� ��������������. ��������� ������ �� �����. 
// ��� ������������� �������� �������� ��������� ������������ �������� ����������� ������.
void task_12(int N) {
	int* a = new int[N];

	for (int i = 0; i < N; i++) {
		a[i] = rand();
	}

	int max = INT_MIN;

	#pragma omp parallel for firstprivate(a) num_threads(4)
	for (int i = 0; i < N; i++) {
		if (a[i] % 7 == 0) {
			#pragma omp critical
			{
				if (max < a[i]) {
					max = a[i];
				}
			}
		}
	}

	printf("Max: %d", max);
}


// ������������� ������ 1 ���, ����� ������ ������������� ���� �������������� � �������� (���������) 
// �������. ���������� ��� ������� 5 �������� �������. ������������ ����� ��� ����� ������. 
void task_13() {
	
}
