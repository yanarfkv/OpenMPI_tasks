#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>


// Написать программу где каждый поток печатает свой идентификатор, количество потоков всего и строчку «Hello World». Запустить программу с 8 потоками. 
// Всегда ли вывод идентичен? Почему?
void task_1() {
	#pragma omp parallel num_threads(8)
	printf("Hello World! Thread ID: %d, threads: %d\n", omp_get_thread_num(), omp_get_num_threads());
}



// Написать программу, в которой определить две параллельные области, выполнение которых зависит от условного оператора #pragma omp parallel if(…), 
// если заданное значение числа нитей больше 2, область выполняется параллельно, иначе не параллельно. Число нитей перед первой областью задать равным 3, 
// перед второй – равным 2. Внутри параллельных областей определить количество нитей и номер каждой нити, результат выдать на экран. 
// Убедиться в правильности работы программы. 
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


// Написать программу, в которой объявить и присвоить начальные значения целочисленным переменным: a и b, определить две параллельные области, 
// первая – с числом нитей 2, вторая – с числом нитей 4. Для первой области переменные a и b объявить защищенными, с режимом доступа private и 
// firstprivate соответственно, внутри области определить номер нити и выполнить увеличение значения переменных на эту величину. Выдать на экран 
// значения переменных до входа в параллельную область, внутри параллельной области и после выхода из параллельной области. Для второй области 
// переменную a объявить общей, переменную b объявить защищенной, с режимом доступа private, определить номер нити и выполнить уменьшение значения 
// переменных на эту величину. Выдать на экран значения переменных до входа в параллельную область, внутри параллельной области и после выхода из 
// параллельной области. Неинициализированные переменные инициализировать.
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


// Написать программу, в которой объявить и присвоить начальные значения целочисленным массивам a[10] и b[10], определить параллельную область, 
// количество нитей задать равным 2, выделить код для основной (номер 0) и нити с номером 1. Основная нить (master) должна выполнять поиск min 
// значения элементов массива a, нить с номером 1 - поиск max значения элементов массива b. Результат выдать на экран.
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


// Написать программу, в которой объявить и присвоить начальные значения элементам двумерного массива d[6][8], для инициализации значений 
// использовать генератор случайных чисел. Используя конструкцию директивы sections…section определить три секции для выполнения следующих операций: 
// первая секция выполняет вычисление среднего арифметического значения элементов двумерного массива,
// вторая секция выполняет вычисление минимального и максимального значений элементов двумерного массива,
// третья секция выполняет вычисление количества элементов массива, числовые значения которых кратны 3.
// В каждой секции определить и выдать на экран номер исполняющей нити и результат выполнения вычислений.
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


// Написать программу, в которой объявить и присвоить начальные значения целочисленному массиву a[100]. Используя конструкцию parallel for и 
// reduction вычислить средние арифметические значения элементов массива a. Одно при помощи reduction, другое без. Сравнить полученные значения. 
// Объяснить возможную разность результатов. 
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



// Написать программу, в которой определить две параллельные области, каждая из которых содержит итерационную конструкцию for выполняющую 
// инициализацию элементов одномерных массивов целых чисел a[12], b[12] и c[12]. Число нитей перед первой областью задать равным 3, 
// перед второй – равным 4. Первая параллельная область выполняет инициализацию элементов массивов a и b с использованием статического 
// распределения итераций, размер порции итераций выбрать самостоятельно, вторая параллельная область выполняет инициализацию элементов 
// массива c по следующему правилу c[i] = a[i] + b[i], с использованием динамического распределения итераций, размер порции итераций выбрать
// самостоятельно. В каждой области определить и выдать на экран количество нитей, номер нити и результат выполнения цикла. Убедиться в 
// правильности работы программы.
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


// Написать программу, в которой объявлен массив из 16000 элементов и инициализирован так, что значение элемента массива равно его 
// порядковому номеру. Затем создайте результирующий массив, в котором (за исключением крайних элементов) будут средние значения исходного массива: 
// b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0
// Запустите программу с 8 - ю процессами при различных типах распределения работ(static, dynamic, guided, auto(runtime, если 
// auto не работает)) и посмотреть время на разных размерах порций.
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



// Используя возможности OpenMP написать программу умножения матрицы на вектор. Сравнить время выполнения последовательной 
// и параллельных программ (выбрать наилучший schedule). Определить размеры матрицы при которых параллельная программа 
// начинает работать быстрей последовательной 
void task_9(int N) {
	
}


// Написать программу, в которой объявить и присвоить начальные значения элементам двумерного массива d[6][8], 
// для инициализации значений использовать генератор случайных чисел. Используя конструкцию директивы omp parallel for и omp critical 
// определить минимальное и максимальное значения элементов двумерного массива. Реализовать аналог reduction 
// для max и min. Количество нитей задать самостоятельно. Результат выдать на экран.
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


// Написать программу, в которой объявить и присвоить начальные значения массиву целых чисел a[30], для 
// инициализации значений использовать генератор случайных чисел. Используя конструкцию omp parallel for и omp atomic 
// вычислить количество элементов массива, числовые значения которых кратны 9. Количество нитей задать самостоятельно. 
// Результат выдать на экран. 
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


// Написать программу, в которой, объявить и заполнить случайными значениями массив целых чисел. 
// Используя возможности OpenMP найти максимальное значение элементов массива кратных 7. Длину 
// массива и количество потоков определить самостоятельно. Результат выдать на экран. 
// Для синхронизации числовых значений максимума используется механизм критических секций.
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


// Модифицируйте задачу 1 так, чтобы потоки распечатывали свои идентификаторы в обратном (убывающем) 
// порядке. Существует как минимум 5 способов решения. Постарайтесь найти как можно больше. 
void task_13() {
	
}
