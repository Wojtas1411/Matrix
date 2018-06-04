#version 330

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//in vec4 iC; //Zmienna interpolowana - kolor wyliczony w vertex shaderze
in vec4 l0; //Wektor do swiatla w przestrzeni oka
in vec4 l1; //Wektor do swiatla w przestrzeni oka
in vec4 n; //Wektor normalny w przestrzeni oka
in vec4 v; //Wektor do obserwatora w przestrzeni oka
in vec2 iTexCoord0; //Wspolrzedne teksturowania

//Zmienne reprezentujace jednostki teksturujace
uniform sampler2D textureMap0;
uniform sampler2D textureMap1;

void main(void) {
    //vec4 ka=vec4(0,0,0,0); //Kolor obiektu w swietle otoczenia
    vec4 kd=texture(textureMap0,iTexCoord0); //Kolor obiektu w swietle rozproszonym
    vec4 ks=texture(textureMap1,iTexCoord0); //Kolor obiektu w swietle odbitym

    vec4 ld0=vec4(1,1,1,0.3); //Kolor swiatla rozpraszanego
    vec4 ld1=vec4(1,1,1,1); //Kolor swiatla rozpraszanego

    vec4 la0=ld0*0.15; //Kolor œwiat³a otoczenia
    vec4 ls0=vec4(1,1,1,0); //Kolor swiatla odbijanego

    vec4 la1=ld1*0.15; //Kolor œwiat³a otoczenia
    vec4 ls1=vec4(0.5,0.7,0.7,0); //Kolor swiatla odbijanego

    vec4 ml0=normalize(l0);
    vec4 ml1=normalize(l1);
    vec4 mn=normalize(n);
    vec4 mv=normalize(v);
    vec4 mr0=reflect(-ml0,mn);
    vec4 mr1=reflect(-ml1,mn);

    float nl0=max(dot(mn,ml0),0); //Kosinus kata pomiedzy wektorami do swiatla i normalnym
    float rv0=pow(max(dot(mr0,mv),0),10); //Kosinus kata pomiedzy wektorami odbitym i do obserwatora do potegi 10

    float nl1=max(dot(mn,ml1),0); //Kosinus kata pomiedzy wektorami do swiatla i normalnym
    float rv1=pow(max(dot(mr1,mv),0),10); //Kosinus kata pomiedzy wektorami odbitym i do obserwatora do potegi 10

	//pixelColor0=ka*la+kd*ld*vec4(nl,nl,nl,1)+ks*ls*rv; //Model oswietlenia Phonga

	vec4 pixelColor0=kd*la0+kd*ld0*vec4(nl0,nl0,nl0,1)+ks*ls0*rv0; //Model oswietlenia Phonga
	vec4 pixelColor1=kd*la0+kd*ld1*vec4(nl1,nl1,nl1,1)+ks*ls1*rv1;
	pixelColor=pixelColor0/4+pixelColor1*4/3;
}
