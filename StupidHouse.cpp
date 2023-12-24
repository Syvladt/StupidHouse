#include<iostream>
#include<string>
#include<algorithm>
#include <sstream>

enum switches
{
	LIGHTS_INSIDE = 1,
	LIGHTS_OUTSIDE = 2,
	HEATERS = 4,
	WATER_PIPE_HEATING = 8,
	CONDITIONER = 16
};

bool check_input(int out_temp, int ins_temp, std::string move, std::string lights)
{
	if ((out_temp < -50 || out_temp > 50) || (ins_temp < -50 || ins_temp > 50))
	{
		std::cout << "Превышены температурные пороги.\n";
		return false;
	}
	else if (move != "YES" && move != "NO")
	{
		std::cout << "Введён некорректный параметр движения.\n";
		return false;
	}
	else if (lights != "ON" && lights != "OFF")
	{
		std::cout << "Недопустимое значение для освещения.\n";
		return false;
	}
	else
		return true;
}

void print_state(int switch_state, int color_temp)
{
	std::cout << "\nСВОДКА:\n";
	if (switch_state & HEATERS)
		std::cout << "Отопление включено\n";
	else
		std::cout << "Отопление выключено\n";
	if (switch_state & CONDITIONER)
		std::cout << "Кондиционер включен\n";
	else
		std::cout << "Кондиционер выключен\n";
	if (switch_state & LIGHTS_INSIDE)
		std::cout << "Освещение в доме включено.\nЦветовая температура: " << color_temp << "K\n";
	else
		std::cout << "Освещение в доме выключено.\n";
	if (switch_state & WATER_PIPE_HEATING)
		std::cout << "Отопление внешнее включено.\n";
	else
		std::cout << "Отопление внешнее выключено.\n";
	if (switch_state & LIGHTS_OUTSIDE)
		std::cout << "Освещение во дворе включено.\n";
	else
		std::cout << "Освещение во дворе выключено.\n";
	std::cout << std::string(53, '=') << "\n";
}

int main()
{
/* Добавить проверку температуры, Кондиционер не выключается при переходе с 40 на 20*/
	setlocale(LC_ALL, "ru");
	std::cout << "Умный дом!\nКуда девался СпейсИнженеринг?\n";
	int switches_state = 0, cur_time, ins_temp, out_temp, color_temp = 5000;
	std::string move, lights;
	for(int day = 1; day <= 2; ++day)
	{
		for (cur_time = 0; cur_time <= 23; ++cur_time)
		{
			do
			{
				std::cout << "\nДень " << day << ". Текущее время " << cur_time << ":00\n";
				std::cout << "Температура\tТемпература\tДвижение\tСвет\n";
				std::cout << "снаружи\t\tвнутри\n";
				std::cin >> out_temp >> ins_temp >> move >> lights;
				std::transform(move.begin(), move.end(), move.begin(), std::toupper);
				std::transform(lights.begin(), lights.end(), lights.begin(), std::toupper);
			} while (! check_input(out_temp, ins_temp, move, lights));
			
			if (out_temp < 0 && !(switches_state & WATER_PIPE_HEATING))
			{
				std::cout << "Включаем внешнее отопление\n";
				switches_state |= WATER_PIPE_HEATING;
			}

			else if (out_temp > 5 && (switches_state & WATER_PIPE_HEATING))
			{
				std::cout << "Выключаем внешнее отопление\n";
				switches_state &= ~WATER_PIPE_HEATING;
			}

			if (ins_temp < 22 && !(switches_state & HEATERS))
			{
				std::cout << "Включаем отопление в доме\n";
				switches_state |= HEATERS;
			}

			if (ins_temp > 30  && !(switches_state & CONDITIONER))
			{
				std::cout << "Включаем кондиционер\n";
				switches_state |= CONDITIONER;
			}
			if (ins_temp >= 25 && (switches_state & HEATERS))
			{
				std::cout << "Выключаем отопление в доме\n";
				switches_state &= ~HEATERS;
			}

			if (ins_temp <= 25 && (switches_state & CONDITIONER))
			{
				std::cout << "Выключаем кондиционер\n";
				switches_state &= ~CONDITIONER;
			}

			if ((lights) == "ON" && !(switches_state & LIGHTS_INSIDE))
			{
				std::cout << "Включаем освещение в доме\n";
				switches_state |= LIGHTS_INSIDE;
				if (cur_time >= 20)
					color_temp = 2700;
				else if (cur_time >= 19)
					color_temp = 3275;
				else if (cur_time >= 18)
					color_temp = 3850;
				else if (cur_time >= 17)
					color_temp = 4425;
				else if (cur_time >= 16)
					color_temp = 5000;
				else if (cur_time >= 0)
					color_temp = 5000;
				std::cout << "Цветовая температура: " << color_temp << '\n';
			}

			if (lights == "OFF" && (switches_state & LIGHTS_INSIDE))
			{
				switches_state &= ~LIGHTS_INSIDE;
				std::cout << "Выключаем освещение в доме\n";
			}

			if (move == "YES" && ((cur_time > 16 && cur_time <= 23) || (cur_time >= 0 && cur_time < 5)))
			{
				std::cout << "Во дворе появилось движение. Включаем свет во дворе.\n";
				switches_state |= LIGHTS_OUTSIDE;
			}
			if (move == "NO" && (switches_state & LIGHTS_OUTSIDE))
			{
				std::cout << "Выключаем свет во дворе.\n";
				switches_state &= ~LIGHTS_OUTSIDE;
			}
			print_state(switches_state, color_temp);
		}
	}
}