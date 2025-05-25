#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <random>
#include <windows.h>
#include "worldmaphandler.h"
#include "karte.h"
#include "hilfszeug.h"
#include "sequenzen.h"
#include "interactions.h"
#include "minimap.h"
#include "cluster.h"
#include "dialoge.h"
#include "werkzeugmanager.h"
#include "statusbars.h"
#include "constructions.h"
#include "scrshots.h"
#include "gegner.h"
#include "shops.h"
#include "tede.h"
#include "angel.h"
#include "enziclop.h"
#include "smirror.h"
#include "kalender.h"
#include "schalter.h"
#include "album.h"
#include "skibook.h"
#include "wizbook.h"
#include "waffenmanager.h"
#include "flyinsects.h"
#include "homeui.h"
#include "optionscreen.h"
#include "introclass.h"
#include "cooking.h"
#include "specialquplayer.h"

using namespace sf;

//TODO
/*   

!!!textureupdate buggy wenn taste gedrückt und andere noch nicht losgelassen
minimap
mayor textur
hold items
323 zauberstab von werkzeug zu waffen wechseln !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
mouse setvisible bei ESC -.-
*/
//BUGS
/*

map wechseln nach falscher viewport
juliatexturen wechseln

*/


int main()
{
	///debug-bing///
	SoundBuffer bingbuf;
	Sound bing;
	SYSTEMTIME st;
	bingbuf.loadFromFile("sounds/dingding.wav");
	bing.setBuffer(bingbuf);
	/// ///////////////
	enum states { title,intro, scene, game, map, dialogue ,fade, tower,spezquest,pause};
	states gamestate;
	gamestate = intro;
	Clock uhr;
	int framezeit;
	int op;
	int bug = 1;
	float fadetimer;
	int check = 100;
	float mausx=1;
	float mausy;
	int mauskachelx;
	int mauskachely;
	int setstate = 99;
	//int day = 0;
	int mapxout = 0;
	int angelret = 0;
	int behindert = 0;
	int constructret = 0;
	int chestret = 0;
	int questret = 0;
	int slingret = 9999;
	int sbookret = 0;
	int speznr = 0;
	int spezret = 0;
	int fbookret = 0;
	int shopret = 0;
	int optret = 0;
	std::ofstream savestream;
	std::ifstream loadstream;
	std::random_device seed;
	std::mt19937 rgen(seed());
	std::uniform_int_distribution<int> fxpdis(5,13);
	std::uniform_int_distribution<int> cxpdis(1, 4);
	int cxp;
	int fxp;
	karte aktivmap(90, 12, 11);
	player julia;
	worldmaphandler worldmap;
	npcs npc;
	cluster clusters;
	hilfszeug hilfe;
	sequenzen sequenz;
	quests quest;
	interactions interact;
	minimap mini;
	dialoge dialog;
	inventar chest;
	werkzeugmanager werkzeug;
	waffenmanager waffen;
	statusbars statusbar;
	constructions construct;
	scrshots screenshot;
	schuss schus;
	RectangleShape fader;
	gegner monster;
	shops shop;
	plants pflanzen;
	trees baum;
	tede turm;
	pop poop;
	angel angeln;
	enziclop lexikon;
	album fbook;
	skibook sbook;
	wizbook wbook;
	smirror spiegel;
	kalender days;
	schalter schalters;
	flyinsects finsekten;
	homeui home;
	optionscreen options;
	introclass intron;
	cooking kochen;
	specialquestplayer specialquest;
	home.kochpoint = &kochen;
	Vector2f vportspeicher;
	Vector3i homeret;
	bool hilf = false;
	bool teston = false;
	bool teston2 = false;
	bool addgreencan = false;
	int click = 0;
	int ruck = 99;
	//std::cout << sizeof(plants::namen)<<std::endl<<sizeof(int)<<std::endl;
	fader.setSize(Vector2f(2560, 1440));
	fader.setFillColor(Color(0, 0, 0, 155));
	fadetimer = 0;
	aktivmap.laden("start100x70.mpl", 100, 70, "start.oly");
	aktivmap.setsheet("sheet.png");
	aktivmap.setoversheet("olysheet.png", 12, 6);
	worldmap.init();
	npc.init();
	quest.init();
	clusters.init();
	for (auto &ding:clusters.nugget)
	{
		if ((ding.name == "semibaum") || (ding.name == "laubbaum") || (ding.name == "nadelbaum"))
		{
			worldmap.karten[ding.karte].auflage[ding.posx + 1][ding.posy] = 1;
		}
	}
	quest.tafelx = clusters.nugget[27].posx;
	quest.tafely = clusters.nugget[27].posy;
	chest.init(werkzeug.hold);
	npc.setgifts(chest);
	werkzeug.init();
	waffen.init();
	construct.init();
	monster.typinit();
	monster.setinit();
	shop.init();
	pflanzen.init();
	kochen.init();
	turm.init();
	lexikon.init(chest,pflanzen,kochen);
	fbook.init(npc.figur);
	sbook.init();
	wbook.init();
	schalters.init();
	finsekten.init(worldmap.karten);
	mini.init(worldmap.karten);
	home.init();
	home.verlinken(julia.onscrx, julia.onscry, aktivmap.viewx, aktivmap.viewy,chest.fridgeopen,chest.fridgex,chest.fridgey,kochen.kochenan);
	sbook.verlinken(julia.masonlvl, julia.masonxp,julia.masonxpmax, julia.farmlvl, julia.farmxp,julia.farmxpmax, julia.fishlvl, julia.fishxp,julia.fishxpmax,julia.masonpnt,julia.farmpnt,julia.fishpnt,julia.cooklvl,julia.cookxp,julia.cookxpmax,julia.cookpnt);
	quest.verlinken(chest.fishoday, chest.recipeoday, chest.produceoday,chest.gemoday);
	options.verlinken(chest.largebar, chest.showbar,schalters.cheatevent);
	op = worldmap.aktuell;
	npc.invsize = chest.invtot;

//	std::vector<int>huibu;
/////////////////////////////
////TESTEINSTELLUNGEN///////
	if(teston)
	{
		quest.book[0].done = true;
		quest.book[0].active = false;
		quest.book[1].active = true;
		quest.book[2].active = true;
		quest.refresh();
		quest.book[1].done = true;
		quest.book[1].active = false;
		quest.book[3].active = true;
		quest.book[4].active = true;
		quest.book[5].active = true;
		//quest.book[6].active = true;
		quest.book[6].done = true;
		quest.book[57].active = true;
		quest.book[7].active = true;
		quest.refresh();
		quest.book[3].done = true;
		quest.book[3].active = false;
		quest.book[8].active = true;
		werkzeug.add(8, "hammer");
		quest.refresh();
		quest.book[2].active = false;
		quest.book[2].done = true;
		quest.book[9].active = true;
		quest.refresh();
		quest.book[9].active = false;
		quest.book[9].done = true;
		quest.book[10].active = true;
		quest.refresh();
		quest.book[4].active = false;
		quest.book[4].done = true;
		quest.refresh();
		npc.figur[5].allowmove = true;
		werkzeug.add(2, "axe");
		quest.book[10].active = false;
		quest.book[10].done = true;
		quest.book[11].active = true;
		quest.refresh();
		quest.book[11].active = false;
		quest.book[11].done = true;
		quest.book[12].active = true;
		quest.book[17].active = true;
		//quest.book[12].active = false;
		//quest.book[12].done = true;
		quest.book[14].active = true;
		quest.book[28].active = true;
		quest.book[31].active = true;
		quest.book[46].active = true;
		quest.refresh();
		julia.pmanamax = 20;
		julia.pmana = julia.pmanamax;
		//quest.book[13].goal = 20;
		//quest.book[13].counter = 0;
		quest.refresh();
		werkzeug.add(9, "wizard staff");
		quest.book[5].active = false;
		quest.book[5].done = true;
		quest.book[16].active = true;
		quest.refresh();
		npc.figur[6].allowmove = true;
		werkzeug.add(3, "shovel");
		werkzeug.add(5, "can");
		werkzeug.add(10, "rod");
		werkzeug.add(6, "small hammer");
		werkzeug.add(7, "Hammer");
		waffen.add(1, "spear");
		waffen.add(2, "battleaxe");
		waffen.add(5, "purplemagic");
		waffen.add(3, "sword");
		worldmap.karten[4].exits[1].open = true;
		npc.figur[2].allowmove = true;
		npc.figur[1].allowmove = true;
		npc.figur[4].allowmove = true;
		npc.figur[1].hlike = 1;
		npc.figur[2].hlike = 2;
		npc.figur[4].hlike = 3;
		npc.figur[5].hlike = 4;
		npc.figur[6].hlike = 5;
		schalters.scene21 = true;
		gamestate = game;
//		huibu.push_back(3);
//		huibu.push_back(3);
//		huibu.push_back(3);
//		huibu.push_back(3);
//		huibu.push_back(3);
//		huibu.push_back(3);
//		huibu[4] = 87;
//		huibu.resize(5);
		//quest.book[20].param = 13;
		//werkzeug.eintrag.erase(werkzeug.eintrag.begin() + 2);
		//werkzeug.eintrag.erase(werkzeug.eintrag.begin() + 2);
		//julia.energie = 5;
		/*
		chest.inhalt[10][5].nr = 1002; //pflaume
		chest.inhalt[10][5].anzahl = 876;
		chest.inhalt[3][3].nr = 1003;  //apfel
		chest.inhalt[3][3].anzahl = 1;
		chest.inhalt[5][2].nr = 1037;  
		chest.inhalt[5][2].anzahl = 2;                           ///chestanfang
		chest.inhalt[2][1].nr = 1024;
		chest.inhalt[2][1].anzahl = 12;
		chest.inhalt[6][2].nr = 1022;
		chest.inhalt[6][2].anzahl = 3;
		chest.inhalt[1][1].nr = 1021;
		chest.inhalt[1][1].anzahl = 5;
		chest.inhalt[1][2].nr = 1008;
		chest.inhalt[1][2].anzahl = 27;*/

		int blub = -1;
		for (int yo = 43;yo < 56;yo++)
		{
			blub += 1;
	//		chest.inhalt[blub % 6][blub / 6].nr = yo;
	//		chest.inhalt[blub % 6][blub / 6].anzahl = 1;
		}


		julia.energie = 40;
		julia.energiemax = 40;
		julia.onmapx = 5400;
		julia.onmapy = 4400;
		aktivmap.viewx = 4200;
		aktivmap.viewy = 3500;
		julia.onscrx = julia.onmapx - aktivmap.viewx;
		julia.onscry = julia.onmapy - aktivmap.viewy;
		julia.setpos();
		//day = 1;
		days.nummer = 1;
		op = 0;
		worldmap.aktuell = 0;
		//aktivmap.setsheet("sheet3.png", 90);
		worldmap.karten[4].auflage[13][3] = 108;
		worldmap.karten[4].auflage[13][5] = 109;
		worldmap.karten[8].auflage[3][2] = 109;
	}
	if (teston2)
	{


		chest.addtocart(1066, 13);
		chest.addtocart(1105, 1);
		chest.addtocart(1008, 50);
		chest.addtocart(1006, 50);
		chest.addtocart(1044, 10);

	}
////////////////////////////

	RenderWindow window(VideoMode::getDesktopMode(/*2560,14401366, 768*/), "legends",Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	//window.setKeyRepeatEnabled(false);
	uhr.restart();
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape)
				{
					setstate = dialog.start(0,1500,600,Color::White,0,"");
					gamestate = dialogue; /// ???
					setstate = 99;
					window.setMouseCursorVisible(true);/// ////////////////////////////////////  entfernen MOUSECURSOR !!!!!!!! du opfer endlich MOUSECURSOR !!!!!
				}
				if (event.key.code == Keyboard::Z)
				{
					//julia.kasten.left = 810;
					//julia.figur.setTextureRect(julia.kasten);
					for (int i = 0;i < 100;i++)
						for (int k = 0;k < 70;k++)
							if (worldmap.karten[4].gitter[i][k] == 8)worldmap.karten[4].gitter[i][k] = 9;
				}
				if (event.key.code == Keyboard::H)
					if (hilf) hilf = false;else hilf = true;
				if (event.key.code == Keyboard::G)
					if (aktivmap.stop) aktivmap.stop = false;else aktivmap.stop = true;
				if (event.key.code == Keyboard::V)
				{
					julia.energie = julia.energiemax;
					gamestate = fade;
				}
				if (event.key.code == Keyboard::P) screenshot.take(window);
				if (event.key.code == Keyboard::T) Mouse::setPosition(Vector2i(1265, 490));
			}
			if (gamestate == intro)
			{
				if (intron.eventupdate(event))
				{
					gamestate = game;
					setstate = 1;
					sequenz.nr = 0;
				}
			}
			if (gamestate == game)
			{
				if(options.anschalten(event))setstate=5;
				click = julia.eventupdate(event, op, worldmap.karten[op].exits,dialog.twice); if (click == 0) bug = 0;
				if (click == 13)/* || (click == 20) || (click == 21) || (click == 22))*/ { setstate = dialog.start(13, julia.onscrx, julia.onscry, julia.farbe, 0,"");click = 99; }///closed door
				questret=quest.eventupdate(event);
				/// /////////////////////////////////////////////////////////////////
				///       DAILY-QUEST-SETUP                                      ///
				/// ////////////////////////////////////////////////////////////////
				if (questret > 0)
				{
					switch (questret)
					{
					case 25:
					{
						worldmap.addobjectmap(1, 150, 99, 69, 0);
					}break;
					case 30:
					{
						worldmap.addobjectmap(1, 151, 99, 69, 0);
					}break;
					default:break;
					}
				}
				/// /////////////////////////////////////////////////////////////////
				chest.eventupdate(event,quest.isopen);
				mini.eventupdate(event);
				werkzeug.eventupdate(event);
				waffen.eventupdate(event);
				lexikon.eventupdate(event);
				fbookret=fbook.eventupdate(event);
				if (fbookret > 0)
				{
					speznr = fbookret;
					setstate = 9;
				}
				/////////////////////////////////////////////////////////////////////
				///       SKILLRETURN                                            ///
				///////////////////////////////////////////////////////////////////
				sbookret=sbook.eventupdate(event);//
				if (sbookret > 0)
				{
					addgreencan = false;
					switch (sbookret)
					{
					case 200:break;
					case 201: {julia.energiemax += 2; julia.energie += 2; }break;
					case 204: {julia.healthmax += 1; julia.health += 1; }break;
					case 206: {julia.pmanamax += 4; julia.pmana += 4; }break;
					case 300: {shop.addangebot(17, 150, 1, 8, 4); shop.typ[1].angebot.push_back(shop.angebotinput); }break;
					case 302:lexikon.rezblatt[16].unlocked = true; break;
					case 303: {shop.addangebot(18, 152, 1, 8, 4); shop.typ[1].angebot.push_back(shop.angebotinput); }break;
					case 306: {shop.addangebot(19, 154, 1, 8, 4); shop.typ[1].angebot.push_back(shop.angebotinput); }break;
					case 307:pflanzen.growbonus += 3; break;
					case 400:addgreencan = true; break;
					case 402:julia.hungermax += 1; break;
					case 403: {julia.healthmax += 1; julia.health += 1; }break;
					case 405: {julia.energiemax += 2; julia.energie += 2; }break;
					case 500:lexikon.rezblatt[15].unlocked = true; break;
					case 501:chest.addtocart(1100, 1); break;
					case 502: {julia.energiemax += 2; julia.energie += 2; }break;
					case 504: {julia.pmanamax += 4; julia.pmana += 4; }break;
					case 509:lexikon.rezblatt[1].unlocked = true; break;
					case 510:lexikon.rezblatt[4].unlocked = true; break;
					case 600: {julia.energiemax += 2; julia.energie += 2; }break;
					case 601:lexikon.rezblatt[9].unlocked = true; break;
					case 603:lexikon.rezblatt[7].unlocked = true; break;
					case 604:lexikon.rezblatt[18].unlocked = true; break;
					case 606:lexikon.rezblatt[19].unlocked = true; break;
					case 608:chest.addtocart(1100, 1); break;
					default:break;
					}
					if (addgreencan)
					{
						int canpos = 0;
						for (size_t i = 0; i < werkzeug.eintrag.size(); i++)
						{
							if (werkzeug.eintrag[i].nr == 5) canpos = i;
						}
						werkzeug.eintrag.erase(werkzeug.eintrag.begin() + canpos);
						werkzeug.add(4, "green can");
						werkzeug.wassermax = 20;
						werkzeug.wasser = 20;
					}
				}
				wbook.eventupdate(event);
				/////////////////////////////////////////////////////////////////////
				///      HOMERETURN                                              ///
				///////////////////////////////////////////////////////////////////
				if (op == 23)homeret=home.eventupdate(event,chest.dragtmp.nr-1000);
				if (homeret.z == 7777)
				{
					int kochtest;
					kochtest = kochen.check();
					if (kochtest > 0)
					{
						if (chest.takefromcart(1100, 1))
						{
							chest.addtocart(kochtest + 1000, 1);
							cxp = cxpdis(rgen);
							julia.cookxp += cxp;
							if (julia.cookxp >= julia.cookxpmax)julia.upgrade(4);

						}
						else setstate = dialog.start(41, julia.onscrx, julia.onscry, julia.farbe, 0, "");
					}
					for (int kap = 1;kap < 10;kap++)
					{
						kochen.kochtopf[kap] = 0;
					}
				}
				if (homeret.z == 9999)
				{
					for (int hugo = 1;hugo < 10;hugo++)
					{
						if (kochen.kochtopf[hugo] > 0)
						{
							chest.addtocart(kochen.kochtopf[hugo] + 1000, 1);
							kochen.kochtopf[hugo] = 0;
						}
					}
				}
				if ((homeret.y > 0)&&(kochen.kochtopf[homeret.x]==0))
				{
					homeret.z=chest.pickorigin;
					kochen.kochtopf[homeret.x] = homeret.y;
					std::cout << "\n" << kochen.kochtopf[homeret.x] << "---" << homeret.x << "---" << homeret.z;
					chest.dragtmp.anzahl -= 1;
					if (chest.dragtmp.anzahl == 0)chest.dragtmp.nr = 1000;

				}
				/////////////////////////////////////////////////////////////////////
				///       CONSTRUCTRETURN                                        ///
				///////////////////////////////////////////////////////////////////
				if (construct.aktiv) constructret=construct.eventupdate(event,chest);
				if (constructret > 0)
				{
					setstate = 1;
					sequenz.nr = constructret;
					if (sequenz.nr == 18)//newhomefestival
					{
						//clusters.nugget[5].nuggetsprite.setTextureRect(IntRect(1080, 0, 540, 540));
						chest.addtocart(1100, 1);
						chest.keys[0].unlocked = true;
						lexikon.rezblatt[0].unlocked = true;
						lexikon.rezblatt[3].unlocked = true;
						lexikon.rezblatt[5].unlocked = true;
						lexikon.rezblatt[11].unlocked = true;
						lexikon.rezblatt[13].unlocked = true;
						clusters.nugget[5].kasten.left = 1080;
						construct.site[0].active = false;
						worldmap.karten[op].auflage[56][55] = 1;
						worldmap.karten[op].auflage[57][55] = 1;
						worldmap.karten[op].auflage[54][59] = 1;
						worldmap.karten[op].auflage[55][59] = 1;
						worldmap.karten[op].auflage[63][57] = 1;
						worldmap.karten[op].auflage[66][59] = 1;
						worldmap.karten[op].auflage[67][59] = 1;
						worldmap.karten[op].auflage[68][59] = 1;
						worldmap.karten[op].auflage[69][54] = 1;
						worldmap.karten[op].auflage[70][54] = 1;
						worldmap.karten[op].auflage[59][54] = 1;
						worldmap.karten[op].auflage[73][55] = 1;
						worldmap.karten[op].auflage[74][55] = 1;
						worldmap.karten[op].auflage[67][55] = 1;
						//worldmap.addexitk(63, 55, worldmaphandler::bereichsname::homeinnen, 23, 90 * 8 + 40, 11 * 90 + 40, true, -550, -200, 0, 0,op);
						worldmap.karten[op].exits[19].x = 63;
						worldmap.karten[op].exits[19].y = 55;
						npc.figur[10].allowmove = true;
						npc.figur[8].allowmove = true;
						aktivmap.viewx = 4355;
						aktivmap.viewy = 4275;
						quest.unlockdailytyp(4);
						quest.book[7].active = false;
						quest.book[7].done = true;
					}
					constructret = 0;
				}
				if (shop.aktiv)
				{
					shopret = shop.eventupdate(event, chest, julia.freeze, schalters.shop1unlock);
					if (shopret == 45)clusters.nugget[7].kasten.left=720;
				}
				if ((waffen.momentan.nr==5)&&(julia.pmana>0))///if zauberstab und mana
				{
					schus.eventupdate(event,julia.onmapx,julia.onmapy,julia.pmana,aktivmap.viewx,aktivmap.viewy);
				}
				/////////////////////////////////////////////////////////////////////
				///       ANGELN                                                 ///
				///////////////////////////////////////////////////////////////////
				if ((werkzeug.momentan.nr == 10) && (((event.type == Event::MouseButtonPressed) && (event.mouseButton.button == Mouse::Left))||((event.type==Event::JoystickButtonPressed)&&(event.joystickButton.button==2))) && (!angeln.aktiv))
				{
					
					mauskachelx = (mausx + aktivmap.viewx) / 90;
					mauskachely = (mausy + aktivmap.viewy) / 90;
					if ((worldmap.karten[op].gitter[mauskachelx][mauskachely] == 31)||(worldmap.karten[op].gitter[mauskachelx][mauskachely] == 148))
					{
						if (julia.energie > 0)
						{
							if (werkzeug.hold > 0)
							{
								if (angeln.setting(julia.onscrx, julia.onscry, event.mouseButton.x, event.mouseButton.y, julia.status, op, schalters.darkmaid, mauskachelx, mauskachely))
								{
									julia.kasten.left = 810;
									julia.energie -= 1;
									chest.takefromcart(werkzeug.hold + 1000, 1);
									if (chest.howmany(werkzeug.hold + 1000) == 0)
										werkzeug.hold = 0;
									julia.freeze = true;
									julia.figur.setTextureRect(julia.kasten);
								}
								else setstate = dialog.start(27, julia.onscrx, julia.onscry, julia.farbe, 0,"");
							}
							else setstate = dialog.start(19, julia.onscrx, julia.onscry, julia.farbe, 0,"");
						}
						else setstate = dialog.start(4, julia.onscrx, julia.onscry, julia.farbe, 0,"");
					}
				}
				if (angeln.aktiv) angeln.eventupdate(event);
				//std::cout << 1;
			}
			if (gamestate == scene) sequenz.eventupdate(event);
			if (gamestate == pause)
			{
				if (options.isopen)
				{
					if (options.anschalten(event) == false)gamestate = game;
					optret=options.eventupdate(event);
					if (schalters.cheatevent)
					{
						schalters.cheatevent = false;
						chest.addtocart(1000 + options.r56, 1);
					}
				}
			}
			if (gamestate == dialogue) 
			{
				if(dialog.eventupdate(event))
				{
					julia.clickx = julia.onscrx;
					julia.clicky = julia.onscry;
					gamestate = game;
					if (dialog.decisionnr == 3) {julia.sleep(statusbar);julia.onmapx = 40*90+45;julia.onmapy = 41*90+20;julia.onscrx = julia.onmapx - aktivmap.viewx;julia.onscry = julia.onmapy - aktivmap.viewy;gamestate = fade;julia.statusneu = player::stati::giu;dialog.decisionnr = 0; }
					if (dialog.decisionnr == 99) {window.close();dialog.decisionnr = 0;}
					if (dialog.decisionnr == 6) { if (!chest.biggerchest(dialog.param))setstate = dialog.start(7, julia.onscrx, julia.onscry, julia.farbe,0,"");dialog.decisionnr = 0; }
					if (dialog.decisionnr == 55) 
					{ 
						if (chest.takefromcart(chest.basilreq + 1000, 1)) 
						{ 
							clusters.nugget[14].nuggetsprite.setTextureRect(IntRect(0, 0, 720, 450));
							clusters.nugget[14].kasten.left = 0;
							worldmap.karten[10].exits[3].open = true;
						}
						else setstate = dialog.start(7, julia.onscrx, julia.onscry, julia.farbe, 0, "");
					}
					if (dialog.decisionnr == 18)
					{
						
						if (npc.checkgifts(werkzeug.hold, npc.zuletzt))
						{
							chest.takefromcart(werkzeug.hold + 1000, 1);
							if (chest.objekte[werkzeug.hold].lexi == inventar::recipe)chest.addtocart(1100, 1);
						}
						else setstate=dialog.start(37, npc.figur[npc.zuletzt].posx - aktivmap.viewx, npc.figur[npc.zuletzt].posy - aktivmap.viewy, npc.figur[npc.zuletzt].farbe, 0, "");
					}
					if (dialog.decisionnr==9)
					{
						if (chest.takefromcart(1009, 1))
						{
							if (chest.takefromcart(1007, 5))
							{
								//////////////quest stonemason II
								quest.book[8].active = false;
								quest.book[8].done = true;
								quest.book[20].active = true;
								quest.refresh();
								werkzeug.add(7, "big hammer");
								setstate = 1;
								sequenz.nr = 15;
							}
							else
							{
								chest.addtocart(1009, 1);
								chest.objekte[6].total -= 1;
								setstate = dialog.start(7, julia.onscrx, julia.onscry, julia.farbe, 0,"");
							}
						}
					}
					if (dialog.decisionnr == 23)
					{
						if (chest.takefromcart(1009, 2))
						{
							if (chest.takefromcart(1007, 8))
							{
								//////////////quest stonemason III
								quest.book[20].active = false;
								quest.book[20].done = true;
								quest.book[58].active = true;
								///quest.book[20].active = true;
								quest.refresh();
								//werkzeug.add(8, "huge hammer");
								setstate = 1;
								sequenz.nr = 27;
							}
							else
							{
								chest.addtocart(1009, 2);
								chest.objekte[6].total -= 2;
								setstate = dialog.start(7, julia.onscrx, julia.onscry, julia.farbe, 0,"");
							}
						}else setstate = dialog.start(7, julia.onscrx, julia.onscry, julia.farbe, 0,"");
					}
					if (dialog.decisionnr == 28)
					{
						if (chest.howmany(1044)>0)
						{
							quest.book[19].active = false;
							quest.book[19].done = true;
							quest.book[23].active = true;
							quest.refresh();
							setstate= dialog.start(30, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 0, "");
						}
						else setstate=dialog.start(29, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 0, "");
					}
					if (dialog.decisionnr == 31)
					{
						if (chest.howmany(1060) > 0)
						{
							quest.book[23].active = false;
							quest.book[23].done = true;
							quest.book[24].active = true;
							quest.refresh();
							setstate = dialog.start(32, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 33, "");
						}
						else setstate=dialog.start(29, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 0, "");
					}
					if (dialog.decisionnr == 34)
					{
						if (chest.howmany(1093) > 0)
						{
							quest.book[24].active = false;
							quest.book[24].done = true;
							quest.refresh();
							//setstate = dialog.start(32, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 33, "");
						}
						else setstate = dialog.start(29, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 0, "");
					}
					if (dialog.decisionnr == 35)
					{
						if (chest.howmany(1031) > 12)
						{
							chest.takefromcart(1031, 13);
							chest.addtocart(1096, 3);
							quest.book[22].active = false;
							quest.book[22].done = true;
							quest.book[47].active = true;
							quest.refresh();
							setstate = 1;
							sequenz.nr = 23;
							//setstate = dialog.start(32, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 33, "");
						}
						else setstate = dialog.start(29, npc.figur[12].posx - aktivmap.viewx, npc.figur[12].posy - aktivmap.viewy, npc.figur[12].farbe, 0, "");
					}
					if (dialog.decisionnr == 42)
					{
						if (chest.howmany(1057) > 14)
						{
							chest.takefromcart(1057, 15);
							//chest.addtocart(1096, 3);
							quest.book[47].active = false;
							quest.book[47].done = true;
							quest.book[48].active = true;
							quest.refresh();
							setstate = 1;
							sequenz.nr = 55;
							angeln.gartensee[0].chance = 760;
							angeln.gartensee[4].chance = 100;
							//setstate = dialog.start(32, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 33, "");
						}
						else setstate = dialog.start(29, npc.figur[12].posx - aktivmap.viewx, npc.figur[12].posy - aktivmap.viewy, npc.figur[12].farbe, 0, "");
					}
					if (dialog.decisionnr == 43)
					{
						if (chest.howmany(1069) > 0)
						{
							chest.takefromcart(1069, 1);
							chest.addtocart(1098, 3);
							quest.book[48].active = false;
							quest.book[48].done = true;
							//quest.book[48].active = true;
							quest.refresh();
							setstate = 1;
							sequenz.nr = 55;
							//setstate = dialog.start(32, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 33, "");
						}
						else setstate = dialog.start(29, npc.figur[12].posx - aktivmap.viewx, npc.figur[12].posy - aktivmap.viewy, npc.figur[12].farbe, 0, "");
					}
					if (dialog.decisionnr == 38)
					{
						check = 1;
						click = 1;
					}
					if (dialog.decisionnr == 39)
					{
						check = 1;
						click = 1;
					}
					if (dialog.decisionnr == 44)
					{
						check = 2;
						click = 2;
					}
					if (dialog.decisionnr == 62)
					{
						check = 3;
						click = 3;
					}
					if (dialog.decisionnr == 45)
					{
						if (chest.howmany(1066) > 0)
						{
							chest.takefromcart(1066, 1);
							//chest.addtocart(1096, 3);
							quest.book[49].active = false;
							quest.book[49].done = true;
							quest.book[50].active = true;
							//quest.book[48].active = true;
							quest.refresh();
							setstate = 1;
							sequenz.nr = 24;
							//angeln.gartensee[0].chance = 760;
							//angeln.gartensee[4].chance = 100;
							//setstate = dialog.start(32, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 33, "");
						}
						else setstate = dialog.start(17, npc.figur[43].posx - aktivmap.viewx, npc.figur[43].posy - aktivmap.viewy, npc.figur[43].farbe, 0, "");
					}
					if (dialog.decisionnr == 60)
					{
						if (chest.howmany(1066) > 2)
						{
							chest.takefromcart(1066, 3);
							//chest.addtocart(1096, 3);
							quest.book[50].active = false;
							quest.book[50].done = true;
							quest.book[51].active = true;
							quest.book[51].param = days.nummer;
							//quest.book[48].active = true;
							quest.refresh();
							setstate = 1;
							sequenz.nr = 25;
							//lexikon.archiblatt[3].unlocked = true;
							//lexikon.archiblatt[4].unlocked = true;
							//angeln.gartensee[0].chance = 760;
							//angeln.gartensee[4].chance = 100;
							//setstate = dialog.start(32, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 33, "");
						}
						else setstate = dialog.start(17, npc.figur[43].posx - aktivmap.viewx, npc.figur[43].posy - aktivmap.viewy, npc.figur[43].farbe, 0, "");
					}
					if (dialog.decisionnr == 40)
					{
						check = worldmap.karten[op].exits.size() - 1;
						click = worldmap.karten[op].exits.size() - 1;
					}
					if (dialog.decisionnr == 10) 
					{ 
						if (chest.takefromcart(1006, 1))
						{
							if (chest.takefromcart(1011, 5))
							{
								////////////quest wizard staff
								quest.book[10].active = false;
								quest.book[10].done = true;
								quest.book[11].active = true;
								quest.book[11].param = days.nummer;
								quest.refresh();
								setstate=dialog.start(12, npc.figur[9].posx - aktivmap.viewx, npc.figur[9].posy - aktivmap.viewy, npc.figur[9].farbe, 0,"");
								npc.figur[9].allowmove = true;

							}
							else
							{
								chest.addtocart(1006, 1);
								chest.objekte[6].total -= 1;
								setstate = dialog.start(7, julia.onscrx, julia.onscry, julia.farbe, 0,"");
							}
						}else setstate = dialog.start(7, julia.onscrx, julia.onscry, julia.farbe, 0,"");
						dialog.decisionnr = 0;
					}
					if (dialog.decisionnr == 14) { shop.aktiv = true;julia.freeze = true;shop.aktivnr = shops::seedshop; dialog.decisionnr = 0; }
					if (dialog.decisionnr == 67) { shop.aktiv = true; julia.freeze = true; shop.aktivnr = shops::redshop; dialog.decisionnr = 0; }
					if (dialog.decisionnr == 68)
					{
						if (chest.takefromcart(1112, 1))
						{
							chest.addtocart(1100, 1);
							quest.book[58].active = false;
							quest.book[58].done = true;
							quest.book[59].active = true;
							quest.refresh();
							setstate = 1;
							sequenz.nr = 28;
						}
						else
						{
							setstate = dialog.start(7, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 0, "");
						}
						dialog.decisionnr = 0;
					}
					if (dialog.decisionnr == 69)
					{
						if (chest.takefromcart(1109, 1))
						{
							chest.addtocart(1100, 1);
							quest.book[59].active = false;
							quest.book[59].done = true;
							quest.book[60].active = true;
							quest.refresh();
							werkzeug.add(8, "huge Hammer");
							setstate = 1;
							sequenz.nr = 55;
						}
						else
						{
							setstate = dialog.start(7, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 0, "");
						}
						dialog.decisionnr = 0;
					}
					if (dialog.decisionnr == 16)
					{
						if (chest.takefromcart(1041, 1))
						{
							quest.book[18].active = false;
							quest.book[18].done = true;
							quest.book[19].active = true;
							quest.unlockdailytyp(3);
							quest.refresh();
							werkzeug.add(10, "simple rod");
							setstate = 1;
							sequenz.nr = 13;
						}
						else
						{
							setstate = dialog.start(17, npc.figur[11].posx - aktivmap.viewx, npc.figur[11].posy - aktivmap.viewy, npc.figur[11].farbe, 0,"");
						}
						dialog.decisionnr = 0;
					}
					if (dialog.twice > 15)
					{
						setstate = dialog.start(dialog.twice, 0, 0,Color::Black, 0,"");
					}
					dialog.decisionnr = 0;
					if (dialog.param > 999)
					{
						julia.statusneu = player::stati::giu;
						julia.textureupdate();
					}
				} //
			}
			if (gamestate==tower)
			{
				turm.eventupdate(event, setstate);
			}
			if (gamestate == spezquest)
			{
				spezret=specialquest.eventupdate(event);
				if (spezret == 555)
				{
					gamestate = game;
				}
			}
			if (event.type == Event::JoystickButtonPressed)
			{
				if (gamestate == game)
				{
					if ((event.joystickButton.button == 3)||(event.joystickButton.button == 3))
					{
						werkzeug.cycler++;
						if (werkzeug.cycler >= werkzeug.eintrag.size())werkzeug.cycler = 0;
						werkzeug.momentan.nr = werkzeug.eintrag[werkzeug.cycler].nr;
						werkzeug.momentan.name = werkzeug.eintrag[werkzeug.cycler].name;
						werkzeug.kasten.left = 90 * werkzeug.momentan.nr;
						werkzeug.momentansprite.setTextureRect(werkzeug.kasten);
						if (werkzeug.momentan.nr == 0) werkzeug.hold = 0;
						werkzeug.hold = 0;
					}
					if ((event.joystickButton.button == 1) || (event.joystickButton.button == 1))
					{
						waffen.cycler++;
						if (waffen.cycler >= waffen.eintrag.size())waffen.cycler = 0;
						waffen.momentan.nr = waffen.eintrag[waffen.cycler].nr;
						waffen.momentan.name = waffen.eintrag[waffen.cycler].name;
						waffen.kasten.left = 90 * waffen.momentan.nr;
						waffen.momentansprite.setTextureRect(waffen.kasten);
					}
					if (event.joystickButton.button == 2)
					{
						interact.buttone = true;
					}
				}
			}
		///baustelle
		}//while pollevent

		framezeit = uhr.restart().asMilliseconds();
		worldmap.update(framezeit);
		mausx = Mouse::getPosition().x;
		mausy = Mouse::getPosition().y;
		if (hilf) hilfe.update(framezeit,mausx, mausy, aktivmap.viewx, aktivmap.viewy, julia.onmapx, julia.onmapy, op, worldmap.karten[worldmap.aktuell].exits.size(), julia.status, julia.statusneu,schus.winkel,julia.pmana,schus.index.size(),pflanzen.nr.size(),npc.figur[11].posx,werkzeug.hold,schalters.shop1unlock);
		
		switch (gamestate)
		{
		case intro:intron.update(framezeit);break;
		case scene:
		{
			if (schalters.gotorita)
			{
				op = 1;
				worldmap.aktuell = 1;
				worldmap.mapwechseln(/*aktivmap*/aktivmap.viewx, aktivmap.viewy, 4, /*julia*/julia.onmapx, julia.onmapy, julia.onscrx, julia.onscry, op, schalters);
				julia.setpos();
				aktivmap.setsheet(worldmap.karten[op].sheetname, worldmap.karten[op].tilesize);
				aktivmap.setoversheet(worldmap.karten[op].oversheet, worldmap.karten[op].oshx, worldmap.karten[op].oshy);
				schalters.gotorita = false;
			}///
			if (schalters.ritatopf)
			{
				schalters.ritatopf = false;
				worldmap.karten[18].auflage[4][3]++;
			}
			if (schalters.archiunlock012)
			{
				lexikon.archiblatt[0].unlocked = true;
				lexikon.archiblatt[1].unlocked = true;
				lexikon.archiblatt[2].unlocked = true;
				schalters.archiunlock012 = false;
			}
			if (schalters.archiunlock34)
			{
				lexikon.archiblatt[3].unlocked = true;
				lexikon.archiblatt[4].unlocked = true;
				schalters.archiunlock34 = false;
			}
			if (schalters.alchyunlock04)
			{
				lexikon.alchyblatt[0].unlocked = true;
				lexikon.alchyblatt[4].unlocked = true;
				chest.keys[1].unlocked = true;
				chest.addtocart(1082, 1);
				schalters.alchyunlock04 = false;
			}
			if (sequenz.update(npc, framezeit, aktivmap.viewx, aktivmap.viewy, julia, schalters) == 0)
			{
				gamestate = game;
				if (schalters.returnview)
				{
					aktivmap.viewx = vportspeicher.x;
					aktivmap.viewy = vportspeicher.y;
					op = 4;
					worldmap.aktuell = 4;
					schalters.returnview = false;
					julia.visible = true;
				}
				if (schalters.festputzen == true)
				{
					schalters.festputzen = false;
					//for (size_t i = 0; i < lexikon.rezblatt.size(); i++)
					//{
					//	lexikon.rezblatt[i].unlocked = true;
					//}
					worldmap.karten[op].auflage[56][55] = 0;
					worldmap.karten[op].auflage[57][55] = 0;
					worldmap.karten[op].auflage[54][59] = 0;
					worldmap.karten[op].auflage[55][59] = 0;
					worldmap.karten[op].auflage[63][57] = 0;
					worldmap.karten[op].auflage[66][59] = 0;
					worldmap.karten[op].auflage[67][59] = 0;
					worldmap.karten[op].auflage[68][59] = 0;
					worldmap.karten[op].auflage[69][54] = 0;
					worldmap.karten[op].auflage[70][54] = 0;
					worldmap.karten[op].auflage[59][54] = 0;
					worldmap.karten[op].auflage[73][55] = 0;
					worldmap.karten[op].auflage[74][55] = 0;
					worldmap.karten[op].auflage[67][55] = 0;
					///worldmap.addexitk
				}
			}
			npc.bewegung(worldmap.karten[op], framezeit);
		}break;
		case game:
		{
			//SAVE
			if (optret > 0)
			{
				savestream.open("savegames/slot" + std::to_string(optret) + ".lmi", std::ios::binary);
				savestream.write(reinterpret_cast<char*>(&op), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.onmapx), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.onmapy), sizeof(int));
				for (int i = 0; i < 12; i++)
					for (int k = 0; k < 8; k++)
					{
						savestream.write(reinterpret_cast<char*>(&chest.inhalt[i][k].nr), sizeof(int));
						savestream.write(reinterpret_cast<char*>(&chest.inhalt[i][k].anzahl), sizeof(int));
					}
				savestream.write(reinterpret_cast<char*>(&chest.chestx), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&chest.chesty), sizeof(int));//
				savestream.write(reinterpret_cast<char*>(&chest.fishoday), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&chest.recipeoday), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&chest.produceoday), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&chest.gemoday), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&chest.basilreq), sizeof(int));
				for (int i = 0; i < quest.book.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&quest.book[i].active), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&quest.book[i].done), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&quest.book[i].param), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&quest.book[i].goal), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&quest.book[i].counter), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&quest.book[i].unlocked), sizeof(bool));
				}
				savestream.write(reinterpret_cast<char*>(&quest.dayzahl), sizeof(int));
				int trenner = 9889;
				int starter = 13887;
				for (int i = 0; i < quest.dayqs.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&quest.dayqs[i]), sizeof(int));
				}
				savestream.write(reinterpret_cast<char*>(&trenner), sizeof(int));
				for (int i = 0; i < werkzeug.eintrag.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&werkzeug.eintrag[i].nr), sizeof(int));
				}
				savestream.write(reinterpret_cast<char*>(&trenner), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&werkzeug.wasser), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&werkzeug.wassermax), sizeof(int));
				for (int i = 0; i < waffen.eintrag.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&waffen.eintrag[i].nr), sizeof(int));
				}
				savestream.write(reinterpret_cast<char*>(&trenner), sizeof(int));
				for (int i = 0; i < chest.objekte.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&chest.objekte[i].total), sizeof(int));
				}
				for (int i = 0; i < chest.keys.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&chest.keys[i].unlocked), sizeof(bool));
				}
				for (int i = 0; i < lexikon.rezblatt.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&lexikon.rezblatt[i].unlocked), sizeof(bool));
				}
				for (int i = 0; i < lexikon.alchyblatt.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&lexikon.alchyblatt[i].unlocked), sizeof(bool));
				}
				for (int i = 0; i < lexikon.archiblatt.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&lexikon.archiblatt[i].unlocked), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&lexikon.archiblatt[i].completed), sizeof(bool));
					for (int k = 0; k < lexikon.archiblatt[i].pieces.size(); k++)
					{
						savestream.write(reinterpret_cast<char*>(&lexikon.archiblatt[i].pieces[k].found), sizeof(bool));
					}
				}
				for (int i = 0; i < npc.figur.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].karte), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].posx), sizeof(float));
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].posy), sizeof(float));
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].allowmove), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].isknown), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].hlike), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].mealunlock), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].gemunlock), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].produceunlock), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].fishunlock), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].favmeal), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].favgem), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].favproduce), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&npc.figur[i].favfish), sizeof(int));
					for (int k = 0; k < npc.figur[i].gifted.size(); k++)
					{
						savestream.write(reinterpret_cast<char*>(&npc.figur[i].gifted[k]), sizeof(int));
					}
					savestream.write(reinterpret_cast<char*>(&trenner), sizeof(int));
				}
				for (int i = 0; i < fbook.friendsblatt.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&fbook.friendsblatt[i].special1), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&fbook.friendsblatt[i].special2), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&fbook.friendsblatt[i].special3), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&fbook.friendsblatt[i].special4), sizeof(int));
				}
				savestream.write(reinterpret_cast<char*>(&julia.healthmax), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.health), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.energiemax), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.energie), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.hungermax), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.hunger), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.pmanamax), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.pmana), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.purplelvl), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.greenlvl), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.bluelvl), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.redlvl), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.whitelvl), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.yellowlvl), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.blacklvl), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.purplexp), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.greenxp), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.bluexp), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.redxp), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.whitexp), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.yellowxp), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.blackxp), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.masonlvl), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.masonxp), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.masonxpmax), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.masonpnt), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.farmlvl), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.farmxp), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.farmxpmax), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.farmpnt), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.fishlvl), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.fishxp), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.fishxpmax), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.fishpnt), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.cooklvl), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.cookxp), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.cookxpmax), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&julia.cookpnt), sizeof(int));
				for (int i = 0; i < worldmap.karten.size(); i++)
				{
					if (worldmap.karten[i].name != worldmaphandler::bereichsname::dungeon)
					{
						for (int k = 0; k < worldmap.karten[i].breite; k++)
						{
							for (int j = 0; j < worldmap.karten[i].hohe; j++)
							{
								savestream.write(reinterpret_cast<char*>(&worldmap.karten[i].gitter[k][j]), sizeof(int));
								savestream.write(reinterpret_cast<char*>(&worldmap.karten[i].auflage[k][j]), sizeof(int));
							}
						}
						for (int m = 0; m < worldmap.karten[i].exits.size(); m++)
						{
							if (worldmap.karten[i].exits[m].door) savestream.write(reinterpret_cast<char*>(&worldmap.karten[i].exits[m].open), sizeof(bool));
						}
					}

				}
				int enumint = 0;
				for (auto& a12 : pflanzen.nr)
				{
					savestream.write(reinterpret_cast<char*>(&a12.knr),sizeof(int));
					savestream.write(reinterpret_cast<char*>(&a12.days), sizeof(int));
					enumint = static_cast<int>(a12.name);
					savestream.write(reinterpret_cast<char*>(&enumint), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&a12.phase), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&a12.reif), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&a12.posx), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&a12.posy), sizeof(int));
				}
				savestream.write(reinterpret_cast<char*>(&trenner), sizeof(int));
				savestream.write(reinterpret_cast<char*>(&pflanzen.growbonus), sizeof(int));
				for (auto& a13 : baum.nr)
				{
					savestream.write(reinterpret_cast<char*>(&a13.karte), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&a13.posx), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&a13.posy), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&a13.fruit), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&a13.illness), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&a13.blooming), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&a13.ready), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&a13.weather), sizeof(int));
				}
				savestream.write(reinterpret_cast<char*>(&trenner), sizeof(int));
				for (int i = 0; i < sbook.masonup.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&sbook.masonup[i].unlocked), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&sbook.masonup[i].upgraded), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&sbook.masonup[i].level), sizeof(int));
				}
				for (int i = 0; i < sbook.farmup.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&sbook.farmup[i].unlocked), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&sbook.farmup[i].upgraded), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&sbook.farmup[i].level), sizeof(int));
				}
				for (int i = 0; i < sbook.farm2up.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&sbook.farm2up[i].unlocked), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&sbook.farm2up[i].upgraded), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&sbook.farm2up[i].level), sizeof(int));
				}
				for (int i = 0; i < sbook.fishup.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&sbook.fishup[i].unlocked), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&sbook.fishup[i].upgraded), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&sbook.fishup[i].level), sizeof(int));
				}
				for (int i = 0; i < sbook.cookup.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&sbook.cookup[i].unlocked), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&sbook.cookup[i].upgraded), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&sbook.cookup[i].level), sizeof(int));
				}
				savestream.write(reinterpret_cast<char*>(&schalters.dorfversperrt), sizeof(bool));
				savestream.write(reinterpret_cast<char*>(&schalters.shop1unlock), sizeof(bool));
				savestream.write(reinterpret_cast<char*>(&schalters.darkmaid), sizeof(bool));
				savestream.write(reinterpret_cast<char*>(&schalters.scene21), sizeof(bool));
				savestream.write(reinterpret_cast<char*>(&days.nummer), sizeof(int));
				for (int i = 0; i < construct.site.size(); i++)
				{
					savestream.write(reinterpret_cast<char*>(&construct.site[i].gesamt), sizeof(int));
					savestream.write(reinterpret_cast<char*>(&construct.site[i].completed), sizeof(bool));
					savestream.write(reinterpret_cast<char*>(&construct.site[i].active), sizeof(bool));
					for (int k = 0; k < construct.site[i].zutat.size(); k++)
					{
						savestream.write(reinterpret_cast<char*>(&construct.site[i].zutat[k].aktuell), sizeof(int));
						savestream.write(reinterpret_cast<char*>(&construct.site[i].zutat[k].quot), sizeof(float));
					}
				}

				savestream.close();
				optret = 0;
			}
			//LADEN
			if (optret < 0)
			{
				optret = optret * -1;
				loadstream.open("savegames/slot" + std::to_string(optret) + ".lmi", std::ios::binary);
				loadstream.read(reinterpret_cast<char*>(&op), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.onmapx), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.onmapy), sizeof(int));
				for (int i = 0; i < 12; i++)
					for (int k = 0; k < 8; k++)
					{
						loadstream.read(reinterpret_cast<char*>(&chest.inhalt[i][k].nr), sizeof(int));
						loadstream.read(reinterpret_cast<char*>(&chest.inhalt[i][k].anzahl), sizeof(int));
					}
				loadstream.read(reinterpret_cast<char*>(&chest.chestx), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&chest.chesty), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&chest.fishoday), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&chest.recipeoday), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&chest.produceoday), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&chest.gemoday), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&chest.basilreq), sizeof(int));
				chest.makechest();
				for (int i = 0; i < quest.book.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&quest.book[i].active), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&quest.book[i].done), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&quest.book[i].param), sizeof(int));
					loadstream.read(reinterpret_cast<char*>(&quest.book[i].goal), sizeof(int));
					loadstream.read(reinterpret_cast<char*>(&quest.book[i].counter), sizeof(int));
					loadstream.read(reinterpret_cast<char*>(&quest.book[i].unlocked), sizeof(bool));
				}
				loadstream.read(reinterpret_cast<char*>(&quest.dayzahl), sizeof(int));
				bool checker = true;
				int leser = 0;
				quest.dayqs.clear();
				while (checker)
				{
					loadstream.read(reinterpret_cast<char*>(&leser), sizeof(int));
					if (leser == 9889)
					{
						checker = false;
					}
					else
					{
						quest.dayqs.push_back(leser);
					}
				}
				checker = true;
				leser = 0;
				werkzeug.eintrag.clear();
				while (checker)
				{
					loadstream.read(reinterpret_cast<char*>(&leser), sizeof(int));
					if (leser == 9889)
					{
						checker = false;
					}
					else
					{
						werkzeug.add(leser, "werkzeug");
					}
				}
				loadstream.read(reinterpret_cast<char*>(&werkzeug.wasser), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&werkzeug.wassermax), sizeof(int));
				checker = true;
				leser = 0;
				waffen.eintrag.clear();
				while (checker)
				{
					loadstream.read(reinterpret_cast<char*>(&leser), sizeof(int));
					if (leser == 9889)
					{
						checker = false;
					}
					else
					{
						waffen.add(leser, "waffe");
					}
				}
				for (int i = 0; i < chest.objekte.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&chest.objekte[i].total), sizeof(int));
				}
				for (int i = 0; i < chest.keys.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&chest.keys[i].unlocked), sizeof(bool));
				}
				for (int i = 0; i < lexikon.rezblatt.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&lexikon.rezblatt[i].unlocked), sizeof(bool));
					//lexikon.rezblatt[i].unlocked = false;///  ////////////////////////////////////////////////////////////////////////////////////////////!!!!!!!!!!!!!!
				}
				for (int i = 0; i < lexikon.alchyblatt.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&lexikon.alchyblatt[i].unlocked), sizeof(bool));
				}
				for (int i = 0; i < lexikon.archiblatt.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&lexikon.archiblatt[i].unlocked), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&lexikon.archiblatt[i].completed), sizeof(bool));
					for (int k = 0; k < lexikon.archiblatt[i].pieces.size(); k++)
					{
						loadstream.read(reinterpret_cast<char*>(&lexikon.archiblatt[i].pieces[k].found), sizeof(bool));
					}
				}
				for (int i = 0; i < npc.figur.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].karte), sizeof(int));
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].posx), sizeof(float));
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].posy), sizeof(float));
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].allowmove), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].isknown), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].hlike), sizeof(int));
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].mealunlock), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].gemunlock), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].produceunlock), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].fishunlock), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].favmeal), sizeof(int));
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].favgem), sizeof(int));
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].favproduce), sizeof(int));
					loadstream.read(reinterpret_cast<char*>(&npc.figur[i].favfish), sizeof(int));
					checker = true;
					leser = 0;
					npc.figur[i].gifted.clear();
					while (checker)
					{
						loadstream.read(reinterpret_cast<char*>(&leser), sizeof(int));
						if (leser == 9889)
						{
							checker = false;
						}
						else
						{
							npc.figur[i].gifted.push_back(leser);
						}
					}
				}
				
				for (int i = 0; i < fbook.friendsblatt.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&fbook.friendsblatt[i].special1), sizeof(int));
					loadstream.read(reinterpret_cast<char*>(&fbook.friendsblatt[i].special2), sizeof(int));
					loadstream.read(reinterpret_cast<char*>(&fbook.friendsblatt[i].special3), sizeof(int));
					loadstream.read(reinterpret_cast<char*>(&fbook.friendsblatt[i].special4), sizeof(int));
				}
				loadstream.read(reinterpret_cast<char*>(&julia.healthmax), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.health), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.energiemax), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.energie), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.hungermax), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.hunger), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.pmanamax), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.pmana), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.purplelvl), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.greenlvl), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.bluelvl), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.redlvl), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.whitelvl), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.yellowlvl), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.blacklvl), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.purplexp), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.greenxp), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.bluexp), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.redxp), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.whitexp), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.yellowxp), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.blackxp), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.masonlvl), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.masonxp), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.masonxpmax), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.masonpnt), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.farmlvl), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.farmxp), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.farmxpmax), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.farmpnt), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.fishlvl), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.fishxp), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.fishxpmax), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.fishpnt), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.cooklvl), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.cookxp), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.cookxpmax), sizeof(int));
				loadstream.read(reinterpret_cast<char*>(&julia.cookpnt), sizeof(int));
				for (int i = 0; i < worldmap.karten.size(); i++)
				{
					if (worldmap.karten[i].name != worldmaphandler::bereichsname::dungeon)
					{
						for (int k = 0; k < worldmap.karten[i].breite; k++)
						{
							for (int j = 0; j < worldmap.karten[i].hohe; j++)
							{
								loadstream.read(reinterpret_cast<char*>(&worldmap.karten[i].gitter[k][j]), sizeof(int));
								loadstream.read(reinterpret_cast<char*>(&worldmap.karten[i].auflage[k][j]), sizeof(int));
							}
						}
						for (int m = 0; m < worldmap.karten[i].exits.size(); m++)
						{
							if (worldmap.karten[i].exits[m].door) loadstream.read(reinterpret_cast<char*>(&worldmap.karten[i].exits[m].open), sizeof(bool));
						}
					}
				}
				plants::plantstruct loadinput;
				checker = true;
				leser = 0;
				int enumcast = 0;
				pflanzen.nr.clear();
				
				while (checker)
				{
					loadstream.read(reinterpret_cast<char*>(&leser), sizeof(int));
					if (leser == 9889)
					{
						checker = false;
					}
					else
					{
						//loadstream.read(reinterpret_cast<char*>(&loadinput.knr), sizeof(int));
						loadinput.knr = leser;
						loadstream.read(reinterpret_cast<char*>(&loadinput.days), sizeof(int));
						loadstream.read(reinterpret_cast<char*>(&enumcast), sizeof(int));
						loadstream.read(reinterpret_cast<char*>(&loadinput.phase), sizeof(int));
						loadstream.read(reinterpret_cast<char*>(&loadinput.reif), sizeof(bool));
						loadstream.read(reinterpret_cast<char*>(&loadinput.posx), sizeof(int));
						loadstream.read(reinterpret_cast<char*>(&loadinput.posy), sizeof(int));
						loadinput.name = static_cast<plants::namen>(enumcast);
						//loadinput.name = plants::namen::grape;
						pflanzen.nr.push_back(loadinput);
					}
				}
				loadstream.read(reinterpret_cast<char*>(&pflanzen.growbonus), sizeof(int));
				trees::treestruct treeinput;
				checker = true;
				leser = 0;
				baum.nr.clear();

				while (checker)
				{
					loadstream.read(reinterpret_cast<char*>(&leser), sizeof(int));
					if (leser == 9889)
					{
						checker = false;
					}
					else
					{
						//loadstream.read(reinterpret_cast<char*>(&loadinput.knr), sizeof(int));
						treeinput.karte = leser;
						loadstream.read(reinterpret_cast<char*>(&treeinput.posx), sizeof(int));
						loadstream.read(reinterpret_cast<char*>(&treeinput.posy), sizeof(int));
						loadstream.read(reinterpret_cast<char*>(&treeinput.fruit), sizeof(int));
						loadstream.read(reinterpret_cast<char*>(&treeinput.illness), sizeof(int));
						loadstream.read(reinterpret_cast<char*>(&treeinput.blooming), sizeof(bool));
						loadstream.read(reinterpret_cast<char*>(&treeinput.ready), sizeof(bool));
						loadstream.read(reinterpret_cast<char*>(&treeinput.weather), sizeof(int));
						baum.nr.push_back(treeinput);
					}
				}
				for (int i = 0; i < sbook.masonup.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&sbook.masonup[i].unlocked), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&sbook.masonup[i].upgraded), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&sbook.masonup[i].level), sizeof(int));
				}
				for (int i = 0; i < sbook.farmup.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&sbook.farmup[i].unlocked), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&sbook.farmup[i].upgraded), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&sbook.farmup[i].level), sizeof(int));
				}
				for (int i = 0; i < sbook.farm2up.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&sbook.farm2up[i].unlocked), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&sbook.farm2up[i].upgraded), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&sbook.farm2up[i].level), sizeof(int));
				}
				for (int i = 0; i < sbook.fishup.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&sbook.fishup[i].unlocked), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&sbook.fishup[i].upgraded), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&sbook.fishup[i].level), sizeof(int));
				}
				for (int i = 0; i < sbook.cookup.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&sbook.cookup[i].unlocked), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&sbook.cookup[i].upgraded), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&sbook.cookup[i].level), sizeof(int));
				}
				loadstream.read(reinterpret_cast<char*>(&schalters.dorfversperrt), sizeof(bool));
				loadstream.read(reinterpret_cast<char*>(&schalters.shop1unlock), sizeof(bool));
				loadstream.read(reinterpret_cast<char*>(&schalters.darkmaid), sizeof(bool));
				loadstream.read(reinterpret_cast<char*>(&schalters.scene21), sizeof(bool));
				loadstream.read(reinterpret_cast<char*>(&days.nummer), sizeof(int));

				for (int i = 0; i < construct.site.size(); i++)
				{
					loadstream.read(reinterpret_cast<char*>(&construct.site[i].gesamt), sizeof(int));
					loadstream.read(reinterpret_cast<char*>(&construct.site[i].completed), sizeof(bool));
					loadstream.read(reinterpret_cast<char*>(&construct.site[i].active), sizeof(bool));
					for (int k = 0; k < construct.site[i].zutat.size(); k++)
					{
						loadstream.read(reinterpret_cast<char*>(&construct.site[i].zutat[k].aktuell), sizeof(int));
						loadstream.read(reinterpret_cast<char*>(&construct.site[i].zutat[k].quot), sizeof(float));
					}
				}
				construct.aktiv = false;

				loadstream.close();

				//////////////////////LOADSTREAM----CLOSED/////////////////////
				if (quest.book[7].done)
				{
					worldmap.karten[op].exits[19].x = 63;
					worldmap.karten[op].exits[19].y = 55;
					clusters.nugget[5].kasten.left = 1080;
				}
				while (shop.typ[1].angebot.size() > 17)
				{
					shop.typ[1].angebot.pop_back();
				}
				if(sbook.farmup[0].upgraded) { shop.addangebot(17, 150, 1, 8, 4); shop.typ[1].angebot.push_back(shop.angebotinput); }
				if (sbook.farmup[3].upgraded) { shop.addangebot(18, 152, 1, 8, 4); shop.typ[1].angebot.push_back(shop.angebotinput); }
				if (sbook.farmup[6].upgraded) { shop.addangebot(19, 154, 1, 8, 4); shop.typ[1].angebot.push_back(shop.angebotinput); }
				days.nummer--;
				days.update();
				quest.refresh();
				std::cout << "------------------";
				std::cout << "savedebug";
				std::cout << op << "\n" << julia.onmapx << "\n" << julia.onmapy;
				std::cout << "-----------------";
				worldmap.aktuell = op;
				aktivmap.viewx = julia.onmapx - 1320;
				aktivmap.viewy = julia.onmapy - 720;
				julia.onscrx = 1320;
				julia.onscry = 720;
				//worldmap.mapwechseln(/*aktivmap*/aktivmap.viewx, aktivmap.viewy, 4, /*julia*/julia.onmapx, julia.onmapy, julia.onscrx, julia.onscry, op, schalters);
				julia.setpos();
				aktivmap.setsheet(worldmap.karten[op].sheetname, worldmap.karten[op].tilesize);
				aktivmap.setoversheet(worldmap.karten[op].oversheet, worldmap.karten[op].oshx, worldmap.karten[op].oshy);
				optret = 0;
			}
			check = julia.input(worldmap.karten[op], framezeit,aktivmap.viewx,aktivmap.viewy,pflanzen);
			if ((click > 0) && (click < 90)) { check = click;click = 99; }
			npc.bewegung(worldmap.karten[op], framezeit);
			monster.bewegung(worldmap.karten[op],op, framezeit);
			ruck=monster.interact(schus.index, op, schus.anzahl, schus.aktiv,worldmap.karten[op]);
			if (ruck < 90)
			{
				schus.index.erase(schus.index.begin() + ruck);schus.anzahl--;if (schus.anzahl == 0)schus.aktiv = false;
				if ((quest.book[13].active)&&(quest.book[13].counter<quest.book[13].goal))quest.book[13].counter++;
			}
			slingret=finsekten.update(op, julia.onmapx, julia.onmapy, aktivmap.viewx, aktivmap.viewy,framezeit);
			if (slingret < 9999)
			{
				finsekten.setting(worldmap.karten[op].blumenx, worldmap.karten[op].blumeny, worldmap.karten[op].occupied, slingret);
			}
			finsekten.flugupdate(framezeit,op);
			werkzeug.update(framezeit);
			waffen.update(framezeit);
			clusters.update(framezeit, op);
			if (op == 23)home.update(mausx,mausy);
			if (schus.aktiv)schus.update(framezeit, aktivmap.viewx, aktivmap.viewy);
			/////////////////////////////////////////////////////////////////////
			///       ANGELN                                                 ///
			///////////////////////////////////////////////////////////////////
			if (angeln.aktiv)angelret=angeln.update(framezeit);
			if (angelret == 666) behindert = 666;
			if ((behindert == 666) && (!angeln.aktiv)) /// DARKMAID found
			{ 
				schalters.darkmaid = true;
				behindert = 0;
				angeln.objekt.setTexture(angeln.objtex);
				setstate = 1;
				sequenz.nr = 17;
				quest.book[22].active = true;
				quest.refresh();

			}
			if (angeln.ende)
			{
				julia.kasten.left = 0;
				julia.figur.setTextureRect(julia.kasten);
				julia.freeze = false;
				angeln.ende = false;
				if (angeln.fang)
				{
					angeln.fang = false;
					julia.fishxp++;
					if ((quest.book[35].active) && (angeln.beute == chest.fishoday))
					{
						quest.book[35].active = false;
						quest.book[35].completed = true;
						quest.book[35].done = true;
						quest.refresh();
						fxp = fxpdis(rgen);
						julia.fishxp += fxp;
					}
					if (julia.fishxp >= julia.fishxpmax)julia.upgrade(3);
					if(!chest.addtocart(angeln.beute+1000,1))
						setstate = dialog.start(2, julia.onscrx, julia.onscry, julia.farbe, 0,"");
				}
				if (angeln.lost)
				{
					poop.setting(angeln.beute, mauskachelx, mauskachely);
				}
			}
			if (poop.aktiv)poop.update(framezeit);
			/////////////////////////////////////////////////////////////////////
			///       MAP WECHSELN                                           ///
			///////////////////////////////////////////////////////////////////
			if (check < 99)
			{ 
				//bing.play();
				mapxout=worldmap.mapwechseln(/*aktivmap*/aktivmap.viewx,aktivmap.viewy, check, /*julia*/julia.onmapx,julia.onmapy,julia.onscrx,julia.onscry, op,schalters);
				julia.setpos();
				aktivmap.setsheet(worldmap.karten[op].sheetname, worldmap.karten[op].tilesize);
				aktivmap.setoversheet(worldmap.karten[op].oversheet, worldmap.karten[op].oshx, worldmap.karten[op].oshy);
				if (mapxout ==14) { setstate = 1;sequenz.nr = mapxout;mapxout = 0; }
				if (mapxout == 4687) { kochen.kochenan = false;chest.fridgeopen = false; mapxout = 0; }
			}
			quest.ui(mausx, mausy,aktivmap.viewx,aktivmap.viewy,op);
			if (lexikon.open) lexikon.ui();
			if (fbook.open) fbook.ui();
			if (sbook.open) sbook.ui();
			if (wbook.open)wbook.ui();
			/*if (chest.isopen)*/ chestret = chest.ui(mausx, mausy, werkzeug.momentan.nr, werkzeug.hold, julia.onscrx, julia.onscry);if(chestret==1) werkzeug.spriteupdate();
			if (chestret > 1000) 
			{ 
				if (julia.hunger < julia.hungermax)
				{
					julia.hunger++;
					julia.consume(chestret);
				}
				else setstate = dialog.start(65, julia.onscrx, julia.onscry, julia.farbe, 0, "");
			}
			if ((click == 0) || (bug == 0)||((interact.buttone)&&(click==0))) { setstate = interact.detect(julia, npc, worldmap.karten[op], quest, chest, clusters, statusbar, sequenz.nr, dialog, werkzeug, construct, days.nummer,pflanzen,aktivmap.viewx,aktivmap.viewy,baum,poop,schalters,waffen);click = 99;bug = 1; }//
			if (setstate == 1) { gamestate = scene; chest.isopen = false; quest.isopen = false; lexikon.open = false; fbook.open = false; sbook.open = false; wbook.open = false; sequenz.start(npc,julia, aktivmap.viewx, aktivmap.viewy); setstate = 99; }
			if (setstate == 3) { gamestate = dialogue; setstate = 99; }
			if (setstate == 5) { gamestate = pause;setstate = 99; }
			if (setstate == 6) { gamestate = tower;window.setMouseCursorVisible(false);setstate = 99; }
			if (setstate == 7) { shop.aktiv = true;julia.freeze = true;shop.aktivnr = shops::stoneshop;setstate = 99; }
			if (setstate == 8) { shop.aktiv = true;julia.freeze = true;shop.aktivnr = shops::fishshop;setstate = 99; }
			if (setstate == 9) { gamestate = spezquest;specialquest.init(speznr);setstate = 99;std::cout << "\n" << speznr << "  speznr"; }
			if ((setstate > 999) && (setstate < 2000)) 
			{
				int archfindret;
				archfindret = lexikon.archifind(setstate - 1000);
				//bing.play();
				if ((archfindret > -90)&&(archfindret<10000))
				{
					dialog.start(56, julia.onscrx, julia.onscry, Color::Black, archfindret, "");
					//bing.play();
					gamestate = dialogue;
					julia.statusneu = player::stati::prendi;
					julia.textureupdate();
				}
				if (archfindret > 10000)
				{
					poop.setting(archfindret - 10000, (julia.clickx + aktivmap.viewx) / 90, (julia.clicky + aktivmap.viewy) / 90);
					chest.addtocart(archfindret - 9000, 1);
					//int kachelx = (x + vx) / 90;
					//int kachely = (y + vy) / 90;
				}
				setstate = 99;
			}
		}break;
		case dialogue:
		{
			if (dialog.aktiv) dialog.update(dialog.x, dialog.y);
		}break;
		case fade:
		{
			fadetimer += framezeit;
			fader.setFillColor(Color(0, 0, 0, (fadetimer / 2000) * 255));
			if (fadetimer > 2000) ///next day
			{
				days.update();
				//worldmap.refresh(aktivmap);
				gamestate = game;
				if ((quest.book[46].active)&&(!schalters.scene21))
				{
					setstate = 1;
					sequenz.nr = 21;
					op = 1;
					julia.visible = false;
					worldmap.aktuell = 1;
					vportspeicher.x = aktivmap.viewx;
					vportspeicher.y = aktivmap.viewy;
					aktivmap.viewx = 15*90;
					aktivmap.viewy = 17*90;
				}
				fadetimer = 0;
				julia.setpos();
				julia.textureupdate();
				//day++;
				baum.nextday();
				worldmap.nextday(days.day);
				pflanzen.nextday(worldmap,baum);
				npc.nextday(days.day);
				clusters.nextday();
				quest.nextday();
				chest.nextday();
				//123
				if (days.nummer == 1)///good morning julia
				{
					setstate = 1;sequenz.nr = 6;
				}
				if (days.nummer == 5)
				{
					///-----------------------------RITASQUEST--------------
					setstate = 1;sequenz.nr = 16;
					quest.book[21].active = true;
					quest.refresh();
				}
				if (days.nummer == 10)
				{
					///-----------------------------LILLYSQUEST---------------
				}
				if (days.day  == 0)//A
				{

					worldmap.karten[4].exits[1].open = true;
					monster.set.clear();
					monster.setinit();
				}
				if (days.day  == 1)//B
				{

					worldmap.karten[4].exits[1].open = false;
				}
				if (days.day  == 2)//C
				{

					worldmap.karten[4].exits[1].open = true;
				}
				if (days.day == 3)//D
				{

					worldmap.karten[4].exits[1].open = false;
				}
				if (days.day  == 4)//E
				{

					worldmap.karten[4].exits[1].open = true;
				}
				if (days.day  == 5)//F
				{

					worldmap.karten[4].exits[1].open = false;
				}
				if (days.day  == 6)//G
				{

					worldmap.karten[4].exits[1].open = true;
				}
			}
		}break;
		case tower:
		{
			turm.update(framezeit);
		}break;
		case spezquest:
		{
			specialquest.update(mausx, mausy, framezeit);
		}break;
		}
		/////////////////////////////////////////////////////////////////////
		///       MALEN                                                  ///
		///////////////////////////////////////////////////////////////////		
		window.clear();
		if ((gamestate != tower)&&(gamestate!=intro)&&(gamestate!=spezquest))
		{
			aktivmap.setviewport(worldmap.karten[op].breite, worldmap.karten[op].hohe);
			aktivmap.malen(window, worldmap.karten[op], julia.onmapx, julia.onmapy, julia, clusters, baum);
			
			clusters.malen(window, op, aktivmap.viewx, aktivmap.viewy,14);
			baum.malen(window, op, aktivmap.viewx, aktivmap.viewy, julia.onmapx, julia.onmapy, 14);
			npc.malen(window, op, aktivmap.viewx, aktivmap.viewy);
			monster.malen(window, op, aktivmap.viewx, aktivmap.viewy);
			if (op == 23)home.malen(window);
			julia.malen(window);
			finsekten.malen(window, op, aktivmap.viewx, aktivmap.viewy);
			clusters.obermalen(window, op, aktivmap.viewx, aktivmap.viewy, 14);
			if (schus.aktiv)schus.malen(window, aktivmap.viewx, aktivmap.viewy);
			if (angeln.aktiv)angeln.malen(window);
			if (poop.aktiv)poop.malen(window, aktivmap.viewx, aktivmap.viewy);
			
			if (quest.book[1].done)days.malen(window);

			if (quest.tafelvisible)quest.pergament.setPosition(quest.tafelx * 90 - aktivmap.viewx +9, quest.tafely * 90 - aktivmap.viewy - 180 - 39);// lol ?!!
			
			quest.malen(window);
			werkzeug.malen(window);
			waffen.malen(window);
			statusbar.malen(window, julia.energie, julia.energiemax, julia.health, julia.healthmax, julia.pmana, julia.pmanamax);
			lexikon.malen(window);
			fbook.malen(window);
			if (quest.book[1].done)sbook.malen(window);
			if (quest.book[12].done)wbook.malen(window);
			///spiegel.malen(window);!!! !!!!! !!!!!!!!

			if (kochen.kochenan)kochen.malen(window);
			if (quest.book[0].done) chest.malen(window);
			if (construct.aktiv) construct.malen(window);
			if (shop.aktiv) shop.malen(window, chest.objekte);
			
			if (quest.book[0].done) mini.malen(window, clusters,angeln, op, julia.onmapx, julia.onmapy);
			if (gamestate == scene) sequenz.malen(window, npc);
			if (gamestate == dialogue) dialog.malen(window);
			if (options.isopen) options.malen(window);
			if (hilf) hilfe.malen(window);
			if (gamestate == fade)window.draw(fader);
		}
		if (gamestate == tower) turm.malen(window,mausx,mausy);
		if (gamestate == spezquest)specialquest.malen(window);
		if (gamestate == intro)intron.malen(window);

		window.display();


	}//while windowisopen
	savestream.open("savegames/savenames.txt");
	for (int i = 0; i < 5; i++)
	{
		savestream << options.slotname[i]<<std::endl;
	}
	savestream.close();

	return 0;
}