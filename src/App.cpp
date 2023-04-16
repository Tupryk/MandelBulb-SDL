#include "App.h"


float map_range(float i, float x_min, float x_max,
				float y_min, float y_max) {
	return y_min + (y_max - y_min) *
	( (i - x_min) / (x_max - x_min) );
}

vec3d rotateVert(vec3d* vert, vec3d* angle, vec3d* rotation_point)
{
    vec3d old_vert = *vert;
    vec3d new_vert;

    new_vert.y = (old_vert.y - rotation_point->y) * cos(angle->x) - (old_vert.z - rotation_point->z) * sin(angle->x) + rotation_point->y;
    new_vert.z = (old_vert.y - rotation_point->y) * sin(angle->x) + (old_vert.z - rotation_point->z) * cos(angle->x) + rotation_point->z;

    old_vert.y = new_vert.y;
    old_vert.z = new_vert.z;

    new_vert.x = (old_vert.x - rotation_point->x) * cos(angle->y) - (old_vert.z - rotation_point->z) * sin(angle->y) + rotation_point->x;
    new_vert.z = (old_vert.x - rotation_point->x) * sin(angle->y) + (old_vert.z - rotation_point->z) * cos(angle->y) + rotation_point->z;

    old_vert.x = new_vert.x;
    old_vert.z = new_vert.z;

    new_vert.x = (old_vert.x - rotation_point->x) * cos(angle->z) - (old_vert.y - rotation_point->y) * sin(angle->z) + rotation_point->x;
    new_vert.y = (old_vert.x - rotation_point->x) * sin(angle->z) + (old_vert.y - rotation_point->y) * cos(angle->z) + rotation_point->y;

    old_vert.x = new_vert.x;
    old_vert.y = new_vert.y;

    return old_vert;
}

bool compare_by_z(const vec3d& a, const vec3d& b) {
    return a.z < b.z;
}

App::App()
{
	std::cout << "Making Bulb..." << std::endl;
    for (float i = 0; i < DIM; i++) {
		for (float j = 0; j < DIM; j++) {

			bool edge = false;
			for (float k = 0; k < DIM; k++) {

				float x = map_range(i, 0, DIM, -1, 1);
				float y = map_range(j, 0, DIM, -1, 1);
				float z = map_range(k, 0, DIM, -1, 1);

				vec3d zeta;

				int index = 0;

				while(true)
				{
					float r = sqrt( zeta.x*zeta.x + zeta.y*zeta.y + zeta.z*zeta.z );
					float theta = atan2( sqrt( zeta.x*zeta.x + zeta.y*zeta.y ), zeta.z );
					float phi = atan2(zeta.y, zeta.x);

					float new_x = pow(r, n) * sin(theta*n) * cos(phi*n);
					float new_y = pow(r, n) * sin(theta*n) * sin(phi*n);
					float new_z = pow(r, n) * cos(theta*n);

					zeta.x = new_x + x;
					zeta.y = new_y + y;
					zeta.z = new_z + z;

					if (r > 2) {
						if (edge) edge = false;
						break;
					}

					index++;
					if (index > max_iterations)
					{
						if (!edge) {
							edge = true;
							vec3d new_point;
							new_point.x = map_range(x, -1, 1, -WINDOW_WIDTH*0.5, WINDOW_WIDTH*0.5);
							new_point.y = map_range(y, -1, 1, -WINDOW_HEIGHT*0.5, WINDOW_HEIGHT*0.5);
							new_point.z = map_range(z, -1, 1, 0.3, 0.6);
							points.push_back(new_point);
						}
						break;
					}
				}
			}
		}
		if ((static_cast<int>(i)+1)%100 == 0)
			std::cout << i+1 << "/" << DIM << std::endl;
	}
	std::cout << "Done!" << std::endl;
}

void App::update()
{
	vec3d angle;
	vec3d rotation_point;
	angle.x = 0.01;
	angle.y = 0.01;
	angle.z = 0.01;

	for (int i = 0; i < points.size(); i++)
	{
		vec3d transformed;
		transformed.x = map_range(points[i].x, -WINDOW_WIDTH*0.5, WINDOW_WIDTH*0.5, -1, 1);
		transformed.y = map_range(points[i].y, -WINDOW_HEIGHT*0.5, WINDOW_HEIGHT*0.5, -1, 1);
		transformed.z = map_range(points[i].z, 0.3, 0.6, -1, 1);

		transformed = rotateVert(&transformed, &angle, &rotation_point);

		points[i].x = map_range(transformed.x, -1, 1, -WINDOW_WIDTH*0.5, WINDOW_WIDTH*0.5);
		points[i].y = map_range(transformed.y, -1, 1, -WINDOW_HEIGHT*0.5, WINDOW_HEIGHT*0.5);
		points[i].z = map_range(transformed.z, -1, 1, 0.3, 0.6);
	}
	std::sort(points.begin(), points.end(), compare_by_z);
}

void App::draw()
{
	for (auto p : points) {
		float bright = 255 * map_range(p.z, 0.3, 0.6, 0.1, 1);
		SDL_SetRenderDrawColor(renderer,
			bright,
			0,
			0,
			255);
		SDL_RenderDrawPoint(renderer,
			p.x*p.z+WINDOW_WIDTH*0.5,
			p.y*p.z+WINDOW_HEIGHT*0.5);
	}
}

void App::run()
{
    SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);

    SDL_Event event;

    while (1)
    {
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		update();
		draw();

		SDL_RenderPresent(renderer);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
