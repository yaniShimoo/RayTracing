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

#pragma region SceneObjects
	Ring ring(
		0.8f,
		1.3f,
		normalize(vector3(0.1f, 1, -0.25f)),
		vector3());


	Sphere planet(.6f, vector3());

	Sphere moon1(
		.2f,
		vector3(.3f, .3f, -1),
		1,
		0.001f,
		//(1 + rand() % 20) * 0.0001f,
		1,
		0.45f,
		4
	);
	Sphere moon2(
		.05f,
		vector3(-0.8f, -0.1f, -4),
		2,
		(1 + rand() % 20) * 0.0001f,
		2,
		-0.9f,
		-7
	);
	Sphere moon3(
		.085f,
		vector3(0, -0.65f, 0),
		1.5f,
		(1 + rand() % 20) * 0.0001f,
		1.5f,
		-0.8f,
		-3
	);
	Sphere moon4(
		.065f,
		vector3(-0.42f, -0.7f, -3.5f),
		1.25f,
		(1 + rand() % 20) * 0.0001f,
		1.25f,
		0.65f,
		-2
	);

	const int SPHERE_ARR_LENGTH = 2;
	//список сфер для рендера
	Sphere spheres[SPHERE_ARR_LENGTH] = {
		planet,
		moon1,
		//moon2,
		//moon3,
		//moon4
	};
#pragma endregion SceneObjects
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
	vector3 lightDir = vector3(5, 3, -5);

	for (int t = 0; t < 1000; t++)
	{
		lightDir = vector3(sin(t * 0.005f) * 5, -3, cos(t * 0.005f) * -5);
		//lightDir = vector3(5, 3, 0.5f + sin(t * 0.005f));
		/*for (int i = 1; i < SPHERE_ARR_LENGTH; i++)
		{
			float angle = t * spheres[i].getOrbitSpeed();
			spheres[i].setPosition(
				vector3(
					spheres[0].getPosition().x + cos(angle) * spheres[i].getTiltX(),
					spheres[0].getPosition().y + cos(angle) * spheres[i].getTiltY(),
					spheres[0].getPosition().z + sin(angle) * spheres[i].getTiltZ()
				));
		}
		*/
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
				HitType hitType = None;
				short hitIndex = -1;
				float tHit;
				for (int k = 0; k < SPHERE_ARR_LENGTH; k++)
				{
					if (sphereIntersect(camPos, normalizedRayDir, spheres[k], tHit)) {
						if (tHit < closestT) {
							closestT = tHit;
							hitIndex = k;
							hitType = SphereType;
						}
					}
				}

				if (ringIntersect(camPos, normalizedRayDir, ring, tHit)) {
					if (tHit < closestT) {
						closestT = tHit;
						hitType = RingType;
					}
				}

				bool isShadow = false;

				if (hitType == SphereType) {
					//точка на координатах, куда попал луч (если т существует)
					vector3 hitPoint = camPos + normalizedRayDir * closestT;

					//Это нормализованный вектор, направленный наружу сферы
					vector3 normal = normalize(hitPoint - spheres[hitIndex].getPosition());

					//тени
					vector3 shadowOrigin = hitPoint + normal * 0.001f;
					if (shadowIntersect(hitPoint, normal, lightDir, spheres, SPHERE_ARR_LENGTH, ring))
						isShadow = true;

					//Яркость точки попадания
					float brightness = dot(normal, lightDir);
					if (brightness < 0)
						brightness = 0;

					// Добавляем глубину пространства
					float ambient = 0.15f;
					float diffuse = fmax(0, brightness);

					float finalRender;

					if (isShadow)
						finalRender = 0;
					else
					{
						brightness = ambient + (1.0f - ambient) * diffuse;
						float depthFactor = clamp(1 - (closestT / 12), 0, 1);
						finalRender = brightness * depthFactor;
					}

					color = (int)(finalRender * 5);

				}
				else if (hitType == RingType)
				{
					vector3 hitPoint = camPos + normalizedRayDir * closestT;
					if (shadowIntersect(hitPoint, ring.getNormal(), lightDir, spheres, SPHERE_ARR_LENGTH, ring))
						isShadow = true;

					float finalRender;
					if (isShadow)
						finalRender = 0;
					else
					{
						float brightness = abs(dot(ring.getNormal(), lightDir));
						float depthFactor = clamp(1 - (tHit / 12), 0, 1);
						finalRender = brightness * depthFactor;
					}

					color = (int)(finalRender * 5);
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
			}
		}
		//"\033[36m
		std::cout << screen;
	}
	delete[] screen;
	return 0;
}