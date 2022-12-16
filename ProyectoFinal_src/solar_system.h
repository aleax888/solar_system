#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sphere.h"
#include <vector>
#include <iostream>

struct solar_system
{
	int to360 = 0;
	
	glm::mat3x3 Rx = glm::mat3x3(1, 0, 0, 0, glm::cos(glm::radians(1.0f)), -glm::sin(glm::radians(1.0f)), 0, glm::sin(glm::radians(1.0f)), glm::cos(glm::radians(1.0f)));
	glm::mat3x3 Ry = glm::mat3x3(glm::cos(glm::radians(1.0f)), 0, glm::sin(glm::radians(1.0f)), 0, 1, 0, -glm::sin(glm::radians(1.0f)), 0, glm::cos(glm::radians(1.0f)));
	glm::mat3x3 Rz = glm::mat3x3(glm::cos(glm::radians(1.0f)), -glm::sin(glm::radians(1.0f)), 0, glm::sin(glm::radians(1.0f)), glm::cos(glm::radians(1.0f)), 0, 0, 0, 1);

	float factor_speed = 4.0f;

	float translate_speeds[9] = { 80.28f, 17.2404f, 12.6108f, 10.7244, 8.6868, 4.7016, 3.4705, 2.4516, 1.9548 };
	float rotation_speeds[9] = { 19.97f, 0.1083, 0.0652, 16.74, 8.66, 0.45583, 0.3684, 0.14794, 0.09719 };

	std::vector<Sphere*> planets;
	std::vector<std::vector<float>> particles;

	std::vector<std::vector<float>> orbits;
	Texture orbit_texture;

	Sphere *sun;
	Sphere *mercury;
	Sphere *venus;
	Sphere *earth;
	Sphere *mars;
	Sphere *jupiter;
	Sphere *saturn;
	Sphere *uranus;
	Sphere *neptune;
	Sphere *stars;

	solar_system(bool realistic = false)
	{
		set_planet_scale(realistic);
		stars = new Sphere("2k_stars_milky_way.jpg", 1000.0f);

		planets.push_back(sun);
		planets.push_back(mercury);
		planets.push_back(venus);
		planets.push_back(earth);
		planets.push_back(mars);
		planets.push_back(jupiter);
		planets.push_back(saturn);
		planets.push_back(uranus);
		planets.push_back(neptune);
		planets.push_back(stars);

		set_planet_distance(realistic);

		update_Rx(-90.0f);
		for (int i = 0, s = planets.size(); i < s; ++i)
		{
			planets[i]->rotation(Rx, planets[i]->center);
		}
		orbit_texture.set_image("white.jpg");
		for (int i = 0; i < 9; ++i)
			particles.push_back(std::vector<float>());
	}

	solar_system(bool realistic, std::vector<glm::vec3> centers)
	{
		set_planet_scale(realistic);
		stars = new Sphere("2k_stars_milky_way.jpg", 1000.0f);

		planets.push_back(sun);
		planets.push_back(mercury);
		planets.push_back(venus);
		planets.push_back(earth);
		planets.push_back(mars);
		planets.push_back(jupiter);
		planets.push_back(saturn);
		planets.push_back(uranus);
		planets.push_back(neptune);
		planets.push_back(stars);

		set_planet_distance(realistic);

		update_Rx(-90.0f);
		for (int i = 0, s = planets.size(); i < s; ++i)
		{
			planets[i]->translation(centers[i]);
			planets[i]->rotation(Rx, planets[i]->center);
		}
		orbit_texture.set_image("white.jpg");
		for (int i = 0; i < 9; ++i)
			particles.push_back(std::vector<float>());
	}

	void set_planet_scale(bool realistic)
	{
		if (realistic)
		{
			sun     = new Sphere("2k_sun.jpg",           6.96);
			mercury = new Sphere("2k_mercury.jpg",       0.024397);
			venus   = new Sphere("2k_venus_surface.jpg", 0.060518);
			earth   = new Sphere("2k_earth_daymap.jpg",  0.06371);
			mars    = new Sphere("2k_mars.jpg",          0.033895);
			jupiter = new Sphere("2k_jupiter.jpg",       0.69911);
			saturn  = new Sphere("2k_saturn.jpg",        0.58232);
			uranus  = new Sphere("2k_uranus.jpg",        0.25362);
			neptune = new Sphere("2k_neptune.jpg",       0.24622);
		}
		else
		{
			sun     = new Sphere("2k_sun.jpg",           6.96);
			mercury = new Sphere("2k_mercury.jpg",       1.4397);
			venus   = new Sphere("2k_venus_surface.jpg", 3.0518);
			earth   = new Sphere("2k_earth_daymap.jpg",  3.371);
			mars    = new Sphere("2k_mars.jpg",          1.8895);
			jupiter = new Sphere("2k_jupiter.jpg",       6.45);
			saturn  = new Sphere("2k_saturn.jpg",        5.4232);
			uranus  = new Sphere("2k_uranus.jpg",        2.2862);
			neptune = new Sphere("2k_neptune.jpg",       2.2622);
		}
	}

	void set_planet_distance(bool realistic)
	{
		if (realistic)
		{
			mercury->translation(glm::vec3(planets[0]->radius + planets[1]->radius + 0.5791f, 0.0f, 0.0f));
			orbits.push_back(generate_cicle_vertices(planets[0]->radius + planets[1]->radius + 0.5791f));
			venus->translation(glm::vec3(planets[0]->radius + planets[2]->radius + 1.082f, 0.0f, 0.0f));
			orbits.push_back(generate_cicle_vertices(planets[0]->radius + planets[2]->radius + 1.082f));
			earth->translation(glm::vec3(planets[0]->radius + planets[3]->radius + 1.466f, 0.0f, 0.0f));
			orbits.push_back(generate_cicle_vertices(planets[0]->radius + planets[3]->radius + 1.466f));
			mars->translation(glm::vec3(planets[0]->radius + planets[4]->radius + 2.2794, 0.0f, 0.0f));
			orbits.push_back(generate_cicle_vertices(planets[0]->radius + planets[4]->radius + 2.2794));
			jupiter->translation(glm::vec3(planets[0]->radius + planets[5]->radius + 7.7833, 0.0f, 0.0f));
			orbits.push_back(generate_cicle_vertices(planets[0]->radius + planets[5]->radius + 7.7833));
			saturn->translation(glm::vec3(planets[0]->radius + planets[6]->radius + 14.294, 0.0f, 0.0f));
			orbits.push_back(generate_cicle_vertices(planets[0]->radius + planets[6]->radius + 14.294));
			uranus->translation(glm::vec3(planets[0]->radius + planets[7]->radius + 28.7099, 0.0f, 0.0f));
			orbits.push_back(generate_cicle_vertices(planets[0]->radius + planets[7]->radius + 28.7099));
			neptune->translation(glm::vec3(planets[0]->radius + planets[8]->radius + 45.043, 0.0f, 0.0f));
			orbits.push_back(generate_cicle_vertices(planets[0]->radius + planets[8]->radius + 45.043));
		}
		else
		{
			float distance = planets[0]->radius;
			for (int i = 1, s = planets.size() - 1; i < s; ++i)
			{
				distance += planets[i]->radius + 1.0f;
				planets[i]->translation(glm::vec3(distance, 0.0f, 0.0f));
				orbits.push_back(generate_cicle_vertices(distance));
				distance += planets[i]->radius;
			}
		}
	}

	void update_Rx(float angle)
	{
		Rx = glm::mat3x3(1, 0, 0, 0, glm::cos(glm::radians(angle)), -glm::sin(glm::radians(angle)), 0, glm::sin(glm::radians(angle)), glm::cos(glm::radians(angle)));
	}

	void update_Ry(float angle)
	{
		Ry = glm::mat3x3(glm::cos(glm::radians(angle)), 0, glm::sin(glm::radians(angle)), 0, 1, 0, -glm::sin(glm::radians(angle)), 0, glm::cos(glm::radians(angle)));
	}

	void update_Rz(float angle)
	{
		Rz = glm::mat3x3(glm::cos(glm::radians(angle)), -glm::sin(glm::radians(angle)), 0, glm::sin(glm::radians(angle)), glm::cos(glm::radians(angle)), 0, 0, 0, 1);
	}

	void heliocentric_model(float deltaTime, float currentFrame, float &particle_time)
	{
		if (currentFrame > particle_time)
		{
			particle_time += 0.05f;
			for (int i = 0, s = planets.size() - 1; i < s; ++i)
			{
				particles[i].push_back(planets[i]->center.x);
				particles[i].push_back(planets[i]->center.y);
				particles[i].push_back(planets[i]->center.z);
			}
		}
		for (int i = 0, s = particles.size(); i < s; ++i)
		{
			orbit_texture.active_texture();
			unsigned int VBO, VAO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, particles[i].size() * sizeof(float), &particles[i][0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glBindVertexArray(VAO);
			//glDrawArrays(GL_LINES, 0, particles[i].size());
			glDrawArrays(GL_LINES, 0, particles[i].size() / 3);
		}
		for (int i = 0, s = planets.size(); i < s; ++i)
		{
			planets[i]->draw();
			planets[i]->translation(glm::vec3(planets[i]->center.x, planets[i]->center.y, planets[i]->center.z - 10.f * deltaTime * factor_speed));
		}
		if (to360 < 360)
		{
			for (int i = 0, s = planets.size() - 1; i < s; ++i)
			{
				update_Rz(translate_speeds[i - 1] * deltaTime * factor_speed);
				planets[i]->rotation(Rz, planets[0]->center);
				update_Ry(rotation_speeds[i - 1] * deltaTime * factor_speed);
				planets[i]->rotation(Ry, planets[i]->center);
			}
			to360++;
		}
		else
		{
			to360 = 0;
		}
	}

	void static_model(float deltaTime)
	{
		
		for (int i = 0, s = planets.size(); i < s; ++i)
		{
			planets[i]->draw();
		}
		for (int i = 0, s = orbits.size(); i < s; ++i)
		{
			orbit_texture.active_texture();
			unsigned int VBO, VAO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, orbits[i].size() * sizeof(float), &orbits[i][0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glBindVertexArray(VAO);
			//glDrawArrays(GL_LINES, 0, orbits[i].size() / 3);
			glDrawArrays(GL_LINE_LOOP, 0, orbits[i].size() / 3);
		}
		if (to360 < 360)
		{
			for (int i = 0, s = planets.size() - 1; i < s; ++i)
			{
				update_Ry(translate_speeds[i - 1] * deltaTime * factor_speed);
				planets[i]->rotation(Ry, planets[0]->center);
				update_Ry(rotation_speeds[i - 1] * deltaTime * factor_speed);
				planets[i]->rotation(Ry, planets[i]->center);
			}
			to360++;
		}
		else
		{
			to360 = 0;
		}
	}

	void over_view(float deltaTime)
	{

	}

	std::vector<float> generate_cicle_vertices(float radius = 0.5f, int amount_vertices = 360)
	{
		std::vector<float> circle_vertices;
		float angle = 0;
		float angle_increment = glm::radians(360.0f / amount_vertices);
		float x = radius, y = 0.0f, z = planets[0]->center.z;
		glm::mat2x2 R;

		for (int i = 0; i < amount_vertices; i++)
		{
			R = glm::mat2x2(glm::cos(angle), -glm::sin(angle), glm::sin(angle), glm::cos(angle));
			circle_vertices.push_back(x * R[0][0] + y * R[0][1]);
			circle_vertices.push_back(x * R[1][0] + y * R[1][1]);
			circle_vertices.push_back(z);
			angle += angle_increment;
		}
		update_Rx(90.0f);
		for (int i = 0; i < amount_vertices * 3; i+=3)
		{
			glm::vec3 aux(circle_vertices[i], circle_vertices[i + 1], circle_vertices[i + 2]);

			aux = Rx * aux;
			//std::cout << planets[0]->center.x << " " << planets[0]->center.y << " " << planets[0]->center.z << std::endl;
			circle_vertices[i] = aux.x;
			circle_vertices[i + 1] = aux.y;
			circle_vertices[i + 2] = aux.z + planets[0]->center.z;
		}

		return circle_vertices;
	}
};


#endif