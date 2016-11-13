#include "Couleur.h"

static std::string indexcolors[] = {
	"#FFFF00", "#1CE6FF", "#FF34FF", "#FF4A46", "#008941", "#006FA6", "#A30059",
	"#FFDBE5", "#7A4900", "#0000A6", "#63FFAC", "#B79762", "#004D43", "#8FB0FF", "#997D87",
	"#5A0007", "#809693", "#FEFFE6", "#1B4400", "#4FC601", "#3B5DFF", "#4A3B53", "#FF2F80",
	"#61615A", "#BA0900", "#6B7900", "#00C2A0", "#FFAA92", "#FF90C9", "#B903AA", "#D16100",
	"#DDEFFF", "#000035", "#7B4F4B", "#A1C299", "#300018", "#0AA6D8", "#013349", "#00846F",
	"#372101", "#FFB500", "#C2FFED", "#A079BF", "#CC0744", "#C0B9B2", "#C2FF99", "#001E09",
	"#00489C", "#6F0062", "#0CBD66", "#EEC3FF", "#456D75", "#B77B68", "#7A87A1", "#788D66",
	"#885578", "#FAD09F", "#FF8A9A", "#D157A0", "#BEC459", "#456648", "#0086ED", "#886F4C",

	"#34362D", "#B4A8BD", "#00A6AA", "#452C2C", "#636375", "#A3C8C9", "#FF913F", "#938A81",
	"#575329", "#00FECF", "#B05B6F", "#8CD0FF", "#3B9700", "#04F757", "#C8A1A1", "#1E6E00",
	"#7900D7", "#A77500", "#6367A9", "#A05837", "#6B002C", "#772600", "#D790FF", "#9B9700",
	"#549E79", "#FFF69F", "#201625", "#72418F", "#BC23FF", "#99ADC0", "#3A2465", "#922329",
	"#5B4534", "#FDE8DC", "#404E55", "#0089A3", "#CB7E98", "#A4E804", "#324E72", "#6A3A4C",
	"#83AB58", "#001C1E", "#D1F7CE", "#004B28", "#C8D0F6", "#A3A489", "#806C66", "#222800",
	"#BF5650", "#E83000", "#66796D", "#DA007C", "#FF1A59", "#8ADBB4", "#1E0200", "#5B4E51",
	"#C895C5", "#320033", "#FF6832", "#66E1D3", "#CFCDAC", "#D0AC94", "#7ED379", "#012C58"
};

Couleur::Couleur(float r, float g, float b, float a) {
	this->Set(r, g, b, a);
};

void Couleur::Set(float r, float g, float b, float a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}
int getValue(char l) {
	switch (l) {
	case '0': return 0; break;
	case '1': return 1; break;
	case '2': return 2; break;
	case '3': return 3; break;
	case '4': return 4; break;
	case '5': return 5; break;
	case '6': return 6; break;
	case '7': return 7; break;
	case '8': return 8; break;
	case '9': return 9; break;
	case 'A': return 10; break;
	case 'B': return 11; break;
	case 'C': return 12; break;
	case 'D': return 13; break;
	case 'E': return 14; break;
	case 'F': return 15; break;

	}
}
void Couleur::Set(int index) {
	assert(index < 255);
	std::string s = indexcolors[index];
	this->r = getValue(s[1]) * 16 + getValue(s[2]);
	this->g = getValue(s[3]) * 16 + getValue(s[4]);
	this->b = getValue(s[5]) * 16 + getValue(s[6]);
	this->a = 1.0f;
}

float* Couleur::Get() const {
	float color[] = { this->r / 255, this->g / 255, this->b / 255, this->a };
	return color;
}

bool operator<(Couleur const &a, Couleur const& b) {
	float* c1 = a.Get();

	float r1 = c1[0];
	float g1 = c1[1];
	float b1 = c1[2];
	float a1 = c1[3];
	float* c2 = b.Get();
	float r2 = c2[0];
	float g2 = c2[1];
	float b2 = c2[2];
	float a2 = c2[3];

	return r1 != r2 ? r1 < r2 : g1 != g2 ? g1 < g2 : b1 != b2 ? b1 < b2 : a1 < a1;
}

bool operator==(Couleur const &a, Couleur const& b) {
	float* c1 = a.Get();

	float r1 = c1[0];
	float g1 = c1[1];
	float b1 = c1[2];
	float a1 = c1[3];
	float* c2 = b.Get();
	float r2 = c2[0];
	float g2 = c2[1];
	float b2 = c2[2];
	float a2 = c2[3];

	return (r1 == r2 && g1 == g2 && b1 == b2 && a1 == a2);
};