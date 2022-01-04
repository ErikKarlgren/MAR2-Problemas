#ifndef HORAS
#define HORAS
#include <iostream>

using namespace std;

class Horas {
private:
	int horas, minutos;
public:
	Horas() : horas(0), minutos(0) {}
	Horas(int hora, int minuto) : horas(hora), minutos(minuto) {}
	int getHora() const {
		return horas;
	}
	int getMinuto() const {
		return minutos;
	}
	void setHora(int hora) {
		horas = hora;
	}
	void setMinuto(int minuto) {
		minutos = minuto;
	}
	bool operator< (const Horas& h) const {
		if (horas < h.getHora()) return true;
		else if (horas == h.getHora()) {
			if (minutos < h.getMinuto()) return true;
			else return false;
		}
		else return false;
	}
	bool operator== (const Horas& h) const {
		if (horas == h.getHora() && minutos == h.getMinuto()) return true;
		else return false;
	}
	Horas operator+ (int mins) const {
		Horas hora;
		hora.setHora(horas);
		hora.setMinuto(minutos + mins);
		if (hora.getMinuto() > 59) {
			hora.setHora(horas + (hora.getMinuto() / 60));
			hora.setMinuto(hora.getMinuto() % 60);
		}
		return hora;
	}
	int toMinutes() {
		return horas * 60 + minutos;
	}
};
inline istream& operator>> (istream& in, Horas& h) {
	int horas, minutos;
	in >> horas;
	in.ignore();
	in >> minutos;
	h = Horas(horas, minutos);
	return in;
}
#endif //HORAS