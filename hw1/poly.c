#include <stdio.h>
#include <stdlib.h>

// 단항식
typedef struct
{
	// 차수
	int exp;
	// 계수
	double coef;
} term_t;

// 다항식
typedef struct
{
	// 다항식의 최구 차수
	int degree;
	// 다항식에 포함된 단항식의 갯수
	int n_terms;
	// array of 단항식
	term_t *terms; // a list of attached terms
} poly_t;

// 빈 다항식을 생성한다.
poly_t *poly_zero()
{
	poly_t *p = (poly_t *)malloc(sizeof(poly_t));

	p->degree = 0;
	p->n_terms = 0;
	p->terms = NULL;
	return p;
}

int poly_degree(poly_t *p)
{
	return p->degree;
}

// specific 차수의 계수를 가져온다.
// O(n)
// validation
// 		단항식의 차수가 음수가 아니어야함.
// 		specific 차수가 다항식의 최고차수보다 작아야함.
double poly_coef(poly_t *p, int exp)
{
	if (exp < 0)
		return 0.0;
	if (p->degree < exp)
		return 0.0;

	int i;
	for (i = 0; i < p->n_terms; i++)
	{
		if ((p->terms[i]).exp == exp)
		{
			return (p->terms[i]).coef;
		}
	}
	return 0.0;
}

// 다항식에 append 단항식
// O(1)
// validation
// 		단항식의 차수가 음수가 아니어야함.
// 		단항식의 계수가 0이 아니어야함
//  	단항식의 차수가 이미 다항식에 존재하지 아니해야함
poly_t *poly_attach(poly_t *p, double coef, int exp)
{
	// !validation이 poly_coef()와 중복됨
	if (exp < 0)
		return NULL;
	if (coef == 0.0)
		return NULL;
	if (poly_coef(p, exp) != 0.0)
		return NULL;

	if (p->degree < exp)
	{
		p->degree = exp;
	}
	p->terms = realloc(p->terms, sizeof(term_t) * (p->n_terms + 1));
	(p->terms[p->n_terms]).coef = coef;
	(p->terms[p->n_terms]).exp = exp;
	p->n_terms += 1;

	return p;
}

// 다항식에서 specific 단항식을 제거한다.
// O(n)
// validation
// 		specific 차수가 음수가 아니어야함.
// 		specific 차수가 다항식의 최고 차수보다 높지 아니해야함.
// 		specific 차수가 다항식에 존재해야함.
poly_t *poly_remove(poly_t *p, int exp)
{
	// !validation이 poly_coef()와 중복됨
	if (exp < 0)
		return NULL;
	if (p->degree < exp)
		return NULL;
	if (poly_coef(p, exp) == 0.0)
		return NULL;

	int i = 0;
	while (p->terms[i].exp != exp)
	{
		i++;
	}
	// assert p->terms[i].exp == exp

	for (; i < p->n_terms - 1; i++)
	{
		p->terms[i] = p->terms[i + 1];
	}
	p->terms = realloc(p->terms, (p->n_terms - 1) * sizeof(term_t));
	p->n_terms -= 1;

	int j;
	p->degree = 0;
	for (j = 0; j < p->n_terms; j++)
	{
		if (p->degree < p->terms[j].exp)
		{
			p->degree = p->terms[j].exp;
		}
	}

	return p;
}

// 다항식과 다항식에 포함된 단항식 array의 memory allocation을 해제한다.
// O(1)
void poly_delete(poly_t *p)
{
	if (p->terms != NULL)
	{
		free(p->terms);
	}
	free(p);
}

// 다항식에 포함된 단항식들의 계수를 scaling한다.
// O(n)
poly_t *poly_scale(poly_t *p, double s)
{
	poly_t *result_p = poly_zero();

	// sacling parameter가 0이라면 연산을 하지않고 다항식을 deallocate하고, empty 다항식을 return한다.
	if (s == 0.0)
	{
		return poly_zero();
	}

	for (int i = 0; i < p->n_terms; i++)
	{
		poly_attach(result_p, p->terms[i].coef * s, p->terms[i].exp);
	}

	return result_p;
	/* TODO */
}

// 두 다항식을 더하는 함수
poly_t *poly_add(poly_t *p1, poly_t *p2)
{
	poly_t *result_p = poly_zero();

	// p1의 단항식을 모두 추가한다.
	for (int i = 0; i < p1->n_terms; i++)
	{
		poly_attach(result_p, p1->terms[i].coef, p1->terms[i].exp);
	}

	// p2의 단항식을 더한다.
	for (int i = 0; i < p2->n_terms; i++)
	{
		// result 다항식에 이미 해당 차수의 단항식이 존재한다면
		if (poly_coef(result_p, p2->terms[i].exp) != 0.0)
		{
			// 찾아서 더한다.
			for (int j = 0; j < result_p->n_terms; j++)
			{
				if (result_p->terms[j].exp == p2->terms[i].exp)
				{
					result_p->terms[j].coef += p2->terms[i].coef;
				}
			}
		}
		// result 다항식에 해당 차수의 단항식이 존재하지 않는다면 추가한다.
		else
		{
			poly_attach(result_p, p2->terms[i].coef, p2->terms[i].exp);
		}
	}

	return result_p;
}

// 두 다항식을 빼는 함수 (p1 - p2)
poly_t *poly_sub(poly_t *p1, poly_t *p2)
{
	poly_t *result_p = poly_zero();

	// p1의 단항식을 모두 추가한다.
	for (int i = 0; i < p1->n_terms; i++)
	{
		poly_attach(result_p, p1->terms[i].coef, p1->terms[i].exp);
	}

	// p2의 단항식을 뺀다.
	for (int i = 0; i < p2->n_terms; i++)
	{
		// result 다항식에 이미 해당 차수의 단항식이 존재한다면
		if (poly_coef(result_p, p2->terms[i].exp) != 0.0)
		{
			// 찾아서 뺀다.
			for (int j = 0; j < result_p->n_terms; j++)
			{
				if (result_p->terms[j].exp == p2->terms[i].exp)
				{
					result_p->terms[j].coef -= p2->terms[i].coef;
				}
			}
		}
		// result 다항식에 해당 차수의 단항식이 존재하지 않는다면 -를 붙여 추가한다.
		else
		{
			poly_attach(result_p, -1 * p2->terms[i].coef, p2->terms[i].exp);
		}
	}

	return result_p;
}

poly_t *poly_mult(poly_t *p1, poly_t *p2)
{
	// 다항식 곱의 결과
	poly_t *result_p = poly_zero();

	// P1의 단항식에 P2의 모든 다항식을 곱한 다항식을 담을 temp poly
	poly_t *temp_p = NULL;

	for (int i = 0; i < p1->n_terms; i++)
	{
		temp_p = poly_zero();

		// P1의 단항식을 가져온다.
		term_t p1Term = p1->terms[i];

		// P1의 단항식을 P2의 다항식에 곱한다.
		for (int j = 0; j < p2->n_terms; j++)
		{
			// P2의 단항식을 가져온다.
			term_t p2Term = p2->terms[j];

			// P1의 단항식과 P2의 단항식을 곱한 결과를 temp poly에 attach한다.
			poly_attach(temp_p, p1Term.coef * p2Term.coef, p1Term.exp + p2Term.exp);
		}

		// 다항식 곱의 결과에 p1의 각 단항식에 p2의 모든 다항식을 곱한 다항식을 더한다.
		result_p = poly_add(result_p, temp_p);
	}

	return result_p;
}

double poly_eval(poly_t *p, double x)
{
	double result = 0.0;
	double acc;

	term_t t;

	for (int i = 0; i < p->n_terms; i++)
	{
		acc = 1.0;
		t = p->terms[i];

		for (int j = 0; j < t.exp; j++)
		{
			acc *= x;
		}

		result += acc * t.coef;
	}

	return result;
}

void poly_show(poly_t *p)
{
	int i;
	for (i = 0; i < p->n_terms; i++)
	{
		printf("%lf", p->terms[i].coef);
		if (p->terms[i].exp != 0)
		{
			printf("x^%d", p->terms[i].exp);
		}
		if (i != p->n_terms - 1)
		{
			if (p->terms[i + 1].coef >= 0.0)
			{
				printf("+");
			}
		}
	}
	printf("\n");
}

int main()
{
	// test poly_scale();
	poly_t *p1 = poly_zero();
	poly_attach(p1, 2.0, 3);
	poly_attach(p1, 4.0, 1);
	poly_attach(p1, 5.0, 0);

	poly_t *p2 = poly_zero();
	poly_attach(p2, 2.0, 2);
	poly_attach(p2, 3.0, 1);
	poly_attach(p2, 7.0, 0);

	poly_t *p3 = poly_zero();
	poly_attach(p3, 1.3, 1);
	poly_attach(p3, 4.0, 3);
	poly_attach(p3, -2.0, 2);
	poly_attach(p3, 3.5, 4);
	poly_attach(p3, 14.2, 0);

	poly_t *p4 = poly_zero();
	poly_attach(p4, 2.0, 5);
	poly_attach(p4, -6.0, 3);
	poly_attach(p4, 7.0, 1);
	poly_attach(p4, 2.1, 4);
	poly_attach(p4, 0, 0);

	printf("p1 : ");
	poly_show(p1);
	printf("p2 : ");
	poly_show(p2);
	printf("p3 : ");
	poly_show(p3);
	printf("p4 : ");
	poly_show(p4);

	/// test poly_scale()
	printf("************** poly_scale ******************");
	printf("\n\n");

	poly_t *poly_scale_result1 = poly_scale(p1, 0.5);
	printf("poly_scale(p1, 0.5) : ");
	poly_show(poly_scale_result1);
	poly_delete(poly_scale_result1);

	poly_t *poly_scale_result2 = poly_scale(p2, 1.2);
	printf("poly_scale(p2, 1.2) : ");
	poly_show(poly_scale_result2);
	poly_delete(poly_scale_result2);

	poly_t *poly_scale_result3 = poly_scale(p3, -3.5);
	printf("poly_scale(p3, -3.5) : ");
	poly_show(poly_scale_result3);
	poly_delete(poly_scale_result3);

	poly_t *poly_scale_result4 = poly_scale(p4, 0.0);
	printf("poly_scale(p4, 0) : ");
	poly_show(poly_scale_result4);
	poly_delete(poly_scale_result4);

	printf("\n\n");

	// test poly_add()
	printf("************** poly_add ******************");
	printf("\n\n");
	poly_t *poly_add_result1 = poly_add(p1, p2);
	poly_t *poly_add_result2 = poly_add(p3, p4);

	printf("poly_add(p1, p2) : ");
	poly_show(poly_add_result1);
	poly_delete(poly_add_result1);

	printf("poly_add(p3, p4) : ");
	poly_show(poly_add_result2);
	poly_delete(poly_add_result2);
	printf("\n\n");

	// test poly_sub()
	printf("************** poly_sub ******************");
	printf("\n\n");
	poly_t *poly_sub_result1 = poly_sub(p1, p2);
	poly_t *poly_sub_result2 = poly_sub(p3, p4);

	printf("poly_sub(p1, p2) : ");
	poly_show(poly_sub_result1);
	poly_delete(poly_sub_result1);

	printf("poly_sub(p3, p4) : ");
	poly_show(poly_sub_result2);
	poly_delete(poly_sub_result2);

	printf("\n\n");

	// test poly_mul()
	printf("************** poly_mul ******************");
	printf("\n\n");
	poly_t *poly_mult_result1 = poly_mult(p1, p2);
	poly_t *poly_mult_result2 = poly_mult(p3, p4);

	printf("poly_mult(p1, p2) : ");
	poly_show(poly_mult_result1);
	poly_delete(poly_mult_result1);

	printf("poly_mult(p3, p4) : ");
	poly_show(poly_mult_result2);
	poly_delete(poly_mult_result2);

	printf("\n\n");

	// test poly_eval()
	printf("************** poly_eval ******************");
	printf("\n\n");
	double poly_eval_result1 = poly_eval(p1, 2.0);
	printf("%lf\n", poly_eval_result1);

	double poly_eval_result2 = poly_eval(p2, 1.4);
	printf("%lf\n", poly_eval_result2);

	double poly_eval_result3 = poly_eval(p3, 5.0);
	printf("%lf\n", poly_eval_result3);

	double poly_eval_result4 = poly_eval(p4, 0.0);
	printf("%lf\n", poly_eval_result4);

	printf("\n\n");

	poly_delete(p1);
	poly_delete(p2);
	poly_delete(p3);
	poly_delete(p4);

	return EXIT_SUCCESS;
}
