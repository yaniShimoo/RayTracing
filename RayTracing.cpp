#include <iostream>
#include <math.h>
#include "vector3.h"
#include "vector_functions.h"
#include "intersaction_functions.h"
#include "models.h"
#include <ctime>


using namespace std;


int main()
{
	srand(time(NULL));

	Ring ring(
		0.95f,
		1.5f,
		normalize(vector3(0.5f, 1, -0.35f)),
		vector3());


	Sphere planet(.6f, vector3());

	Sphere moon1(
		.1f,
		vector3(0.8f, 0.2f, -3),
		1,
		(1 + rand() % 20) * 0.0001f,
		1,
		0.5f,
		-5
	);
	Sphere moon2(
		.05f,
		vector3(-0.8f, -0.1f, -4),
		2,
		(1 + rand() % 20) * 0.0001f,
		2,
		-0.1f,
		3
	);
	Sphere moon3(
		.085f,
		vector3(0, -0.65f, 0),
		1.5f,
		(1 + rand() % 20) * 0.0001f,
		1.5f,
		-0.8f,
		2.5f
	);
	Sphere moon4(
		.065f,
		vector3(-0.42f, -0.7f, -3.5f),
		1.25f,
		(1 + rand() % 20) * 0.0001f,
		1.25f,
		0.65f,
		-6
	);

	const int SPHERE_ARR_LENGTH = 5;
	//список сфер для рендера
	Sphere spheres[SPHERE_ARR_LENGTH] = {
		planet,
		moon1,
		moon2,
		moon3,
		moon4
	};

	//размер консоли (экран)
	const int width = 120, height = 30;
	//пиксели на экране
	char* screen = new char[width * height];
	//экран в 3 раза шире, чем выше.
	// При рисовании координат мы получаем квадратную область.
	// Но экран у нас не квадратный, а широкий. 
	float aspect = (float)width / (float)height;

	//Тоже самое касается и самих символов, которые мы выводим.
	float pixAspect = 11.0f / 24.0f;

	//Градиент символов
	char grad[] = " .:!/r(l1Z4H9W8$@";
	int gradSize = size(grad) - 2;

	float u, v;
	vector3 lightDir = normalize(vector3(2, 1, -2));

	for (int t = 0; t < 80000; t++)
	{
		for (int i = 1; i < SPHERE_ARR_LENGTH; i++)
		{
			float angle = t * spheres[i].getOrbitSpeed();
			spheres[i].setPosition(
				vector3(
					spheres[0].getPosition().x + cos(angle) * spheres[i].getTiltX(),
					spheres[0].getPosition().y + cos(angle) * spheres[i].getTiltY(),
					spheres[0].getPosition().z + sin(angle) * spheres[i].getTiltZ()
				));
		}
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				// нормализация. Переводим все значения в [0...1]
				u = (x + .5f) / (float)width;
				v = (y + .5f) / (float)height;
				//переводим нормализованные координаты в систему координат, то есть [-1...1]
				u = 2.0f * u - 1.0f;
				v = 2.0f * v - 1.0f;
				//учитываем аспект
				u *= aspect * pixAspect;
				//инверт по вертикале(для орейнтации)
				v = -v;

				vector3 screenPoint = vector3(u, v, 0);

				//позиция камеры в пространстве
				vector3 camPos = vector3(0, 0, -10);
				vector3 normalizedRayDir = normalize(screenPoint - camPos);

				int color = 0;

				float closestT = 99999;
				short hitType = -1;
				short hitIndex = -1;
				float tHit;
				for (int k = 0; k < SPHERE_ARR_LENGTH; k++)
				{
					if (sphereIntersect(camPos, normalizedRayDir, spheres[k], tHit)) {
						if (tHit < closestT) {
							closestT = tHit;
							hitIndex = k;
							hitType = 0;
						}
					}
				}

				if (ringIntersect(camPos, normalizedRayDir, ring, tHit)) {
					if (tHit < closestT) {
						closestT = tHit;
						hitType = 1;
					}
				}

				if (hitType == 0) {
					//точка на координатах, куда попал луч (если т существует)
					vector3 hitPoint = camPos + normalizedRayDir * closestT;

					//Это вектор, направленный наружу сферы
					vector3 normal = normalize(hitPoint - spheres[hitIndex].getPosition());

					float brightness = dot(normal, lightDir);
					if (brightness < 0)
						brightness = 0;

					// Добавляем глубину пространства
					float ambient = 0.15f;
					brightness = fmax(0, brightness);
					brightness = ambient + (1.0f - ambient) * brightness;
					float depthFactor = clamp(1 - (closestT / 12), 0, 1);
					float finalRender = brightness * depthFactor;

					color = (int)(finalRender * 20);

				}
				else if (hitType == 1)
				{
					float brightness = abs(dot(ring.getNormal(), lightDir));
					float depthFactor = clamp(1 - (tHit / 12), 0, 1);
					float finalRender = brightness * depthFactor;
					color = (int)(finalRender * 35);
				}
				else {
					char pix = ' ';
					float noise = generateNoise(x, y);
					if (noise > 0.995) pix = '*';
					else if (noise > 0.99) pix = '.';
					*(screen + (int)(y * width) + x) = pix;
					continue;
				}
				color = clamp(color, 0, gradSize);
				*(screen + (int)(y * width) + x) = grad[color];
				//cout << *(screen + (y * width) + x);
			}
		}
		//"\033[36m
		std::cout << screen;
	}
	delete[] screen;
	return 0;
}