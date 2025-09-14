#include "stm32f10x.h"                  // Device header
#include "timer.h" 
#include "usart_car.h"
#include "Delay.h"
#define AIN1 GPIO_Pin_12
#define AIN2 GPIO_Pin_13
#define BIN1 GPIO_Pin_14
#define BIN2 GPIO_Pin_15
#define  HCSR04_PORT_Trig                   GPIOA
#define  HCSR04_PORT_Echo                   GPIOA
#define  HCSR04_CLK_Trig                    RCC_APB2Periph_GPIOA
#define  HCSR04_CLK_Echo                    RCC_APB2Periph_GPIOA
#define  HCSR04_TRIG                        GPIO_Pin_11
#define  HCSR04_ECHO                        GPIO_Pin_15
u16 length;
//extern int time;
void GPIO_Motor_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = AIN1|AIN2|BIN1|BIN2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_Initurec;
	GPIO_Initurec.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Initurec.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_13;
	GPIO_Initurec.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void MPU_exti_init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
 	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);				  //婵炶揪绲藉锟犲极閹捐绾уù鐓庣摠娴溿倝鏌熸搴″幋闁轰焦鎹囬幆宀勵敃閵堝棭浠ч梺褰掝棑閸忔﹢寮幘璇叉闁靛牆妫楅鍫曟煛閸愩劎鍩ｉ柡浣规崌瀵剟濡堕崱妤婁紦
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;								  //PA12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 					    //闂備浇娉曢崰鎰板几婵犳艾绠柣鎴ｅГ閺呮悂鏌ｉ褍浜炴い鎺撶叀楠炲繘鎮滈懞銉︽闂佸搫鍊堕崐鏍偓姘秺閺屻劑鎮㈤崨濠勪紕闂佸綊顥撻崗姗€寮幘璇叉闁靛牆妫楅鍫曟⒑鐠恒劌鏋戦柡瀣煼楠炲繘鏁撻敓锟�
 	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12); //闂備浇娉曢崰鎰板几婵犳艾绠紒瀣12闂佸搫瀚繛鈧柡浣规崌楠炲骞掗幋婵堝帓闂備浇娉曢崰搴ゃ亹閵娧勫閻熸瑥瀚鍫曟⒑鐠恒劌鏋戦柡瀣煼楠炲繘鏁撻敓锟�12
	EXTI_InitStructure.EXTI_Line=EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //闂備浇娉曢崰鏍箺椤忓牆妫橀柕鍫濇椤忓爼姊虹捄銊ユ瀾闁糕晛鎳忓蹇涙倷椤掆偓椤忓爼姊虹捄銊ユ瀾闁哄顭烽獮蹇涙晸閿燂拷
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	  	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	//闂備浇娉曢崰鎰板几婵犳艾绠€瑰嫭婢樼粣妤呮⒑鐠恒劌鏋戦柡瀣煼楠炲繘鎮滈懞銉︽婵°倖顨夊▍锝夊Υ鐎ｎ喖绠柨鐕傛嫹0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			    //闂備浇娉曢崰鎰板几婵犳艾绠€瑰嫭澹嗙€瑰姊虹捄銊ユ瀾闁哄顭烽獮蹇涙倻閼恒儲娅㈡俊銈嗩殙濞咃綁濡寸€ｎ喖绠柨鐕傛嫹0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
	NVIC_Init(&NVIC_InitStructure);
}
void NVIC_Config(void)
{  
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;       
    NVIC_Init(&NVIC_InitStructure);
}
void HC_SR04_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
//   TIM_TimeBaseInitTypeDef TIM_hcsr04init;//闂佽姘﹂～澶屽枈瀹ュ拋娓婚柛宀€鍋涢梻顖炴煏婵犲繘妾い蟻鍥ㄢ拺闁伙絽鑻弸鏃傜磼椤斿搫濮傜€殿喗鎮傚畷鐔碱敂閸℃顏堟⒑閸涘﹤绗氱紒璇插€块敐鐐哄箛閺夎法顦遍梺鍝勭Р閸婃劙宕戦幘缁樻櫢闁跨噦鎷�
	
    RCC_APB2PeriphClockCmd(HCSR04_CLK_Trig | HCSR04_CLK_Echo, ENABLE);      //濠电偠鎻紞鈧柛搴☆煼瀹曟螖缁傛梻绱撴担瑙勵棓闁搞劍濯介妵鎰槈閵忕姷鐫勯梺鍓插亝濞叉鈧鎷�
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);                    //濠电偠鎻紞鈧柛搴☆煼瀹曟螖缂嶎敵2闂備礁鎼崯顐︽偉婵傜ǹ绠伴柨鐕傛嫹
	
    GPIO_InitStructure.GPIO_Pin = HCSR04_TRIG;                              // 闂備礁鍚嬪娆撍囬鐐茬柈閻犳亽鍔嶉崰鍡涙煥閺囨浜鹃悷婊勬緲閸婅崵鍒掗埡鍜佹晬婵﹩鍓涚粙澶愭⒑閹稿海鈽夋繝鈧粊瀹╣闂備焦瀵х粙鎴λ囬悽绋垮偍闁靛牆娲ㄧ壕楣冩煥閻曞倹瀚�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                        //闂備浇顫夋禍浠嬪礉瀹ュ洣鐒婇柧蹇ｅ亝缂嶅洭鏌熼幆褍鏆辨慨锝忔嫹
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                       //IO闂備礁鎲￠悷杈╃礊娓氣偓閸┾偓妞ゆ帒鍊搁弸搴ㄦ倵绾懏鐝紒瀣樀閺佹捇鏁撻敓锟�50MHz
    GPIO_Init(HCSR04_PORT_Trig, &GPIO_InitStructure);                       //闂備礁鎼粔鐑斤綖婢跺﹦鏆ゅù锝囧劋婵ジ鏌熼鍡楁噺濮ｅ酣姊洪崨濠勬噧闁稿﹤缍婂鎶芥偄閻撳海顦┑鐘绘涧濡厼顭囧Δ鍛厱闁哄秲鍔庢晶杈ㄣ亜韫囷絽澧寸€规洩缍侀弫鎾绘晸閿燂拷
    GPIO_ResetBits(HCSR04_PORT_Trig,HCSR04_TRIG);
 
    GPIO_InitStructure.GPIO_Pin = HCSR04_ECHO;                              // 闂備焦鎮堕崕鎶藉磻閻愬吀鐒婃繝闈涱儏缁犳娊鏌曟径娑㈡闁哄棭浜炵槐鎺楀籍閹存繄浠╅悷婊呭閻╊垶寮澶婃嵍婵﹥濮峯闂備焦瀵х粙鎴λ囬悽绋垮偍闁靛牆娲ㄧ壕楣冩煥閻曞倹瀚�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                   //婵犵數鍋為幆宀勫窗濮樿泛鐓橀柡宥冨妽缂嶅洭鏌熼幆褍鏆辩€殿噯鎷�
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                       //IO闂備礁鎲￠悷杈╃礊娓氣偓閸┾偓妞ゆ帒鍊搁弸搴ㄦ倵绾懏鐝紒瀣樀閺佹捇鏁撻敓锟�50MHz
    GPIO_Init(HCSR04_PORT_Echo, &GPIO_InitStructure);                       //闂備礁鎼粔鐑斤綖婢跺﹦鏆ゅù锝囧劋婵ジ鏌熼鍡楁噺濮ｅ酣姊洪崨濠勬噧闁稿﹤缍婂鎶芥偄閻撳海顦┑鐘绘涧濡厼顭囧Δ鍛厱闁哄秲鍔庢晶杈ㄣ亜韫囷絽澧寸€规洩缍侀弫鎾绘晸閿燂拷
 
    //闂佽姘﹂～澶屽枈瀹ュ拋娓婚柛宀€鍋涢梻顖炴煏婵炲灝鍔氶柣蹇嬪劚閳规垿顢欑喊鍗炲壈闁诲骸鐏氶悡锟犲极瀹ュ閱囨繝闈涚墛椤旀厜PB1闂備焦鐪归崝宀€鈧凹鍘奸—鍐磼閻愬鍙冮梺缁樻⒐濡炲潡宕惔锝囩＝闁告劖鍎抽弸鐔兼煕椤帗瀚�72Mhz闂備焦瀵ч崘濠氬箯閿燂拷
    /* TIM_hcsr04init.TIM_ClockDivision = TIM_CKD_DIV1;                        //濠电偞鍨堕幐鍝ョ矓閹绢喖鍨傞幖鎼緛缁憋綁鏌ㄩ悤鍌涘
    TIM_hcsr04init.TIM_CounterMode   = TIM_CounterMode_Up;                  //闂佽崵濮崇拋鏌ュ礈濠靛姹查柣鏃囧仱閻旂厧鐏崇€规洖娲ㄩ、鍛箾閹寸偞灏柟纰卞亰楠炲啫饪伴崟顓涙灃婵犵數濮撮幏瀣触閳ь剟姊烘潪鎵瓘闁瑰嚖鎷�
    TIM_hcsr04init.TIM_Period        = 1000-1;                              //闂傚倷鐒﹁ぐ鍐矓閻戝壙澶嬬節閸愨晛顫￠梺鑺ッˇ顐﹀磻閹剧粯鏅搁柨鐕傛嫹
    TIM_hcsr04init.TIM_Prescaler     = 72-1;                                //濠碘槅鍋呭妯尖偓姘煎墴瀹曡鎯旈～顔剧煑闂佺懓顕繛鈧柛蹇撴喘閺屸剝绗熸繝鍐伓
    TIM_TimeBaseInit(TIM1,&TIM_hcsr04init);                                  //闂傚倷鐒﹀妯肩矓閸洘鍋柛鈩冪憿閸嬫挾鎲撮崟顓ф殹濠碉紕鍋涢崐鍨暦閹惰棄惟闁靛鍎虫禒瀛樼節濞堝灝鏋涙い鎴濇噽閳ь剙鐏氶悡鈥崇暦濞嗘挻鍤嶉柕澶涢檮椤旓拷
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);                                 //婵犵數鍋為幐鎼佸箠韫囨稏鈧啴鎮滅化寤欓梻浣圭湽閸斿瞼鈧凹浜幊娆撳箣閿曗偓濡ɑ銇勯幘璺烘灁闁搞劌銈搁悡顐﹀炊閵婏絼娌紓鍌氱▌閹凤拷
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                                 //濠电偠鎻紞鈧柛搴☆煼瀹曟螣鐏忔牕浜鹃悷娆忓椤ｅ弶淇婇悙鎻掆偓鍨暦閹惰棄惟闁靛牆鐗嗛獮鈧梻浣告惈閸嬪﹪骞忛敓锟�   
    NVIC_Config();                                                          //闂佽崵濮崇粈浣规櫠娴犲鍋柛鈩冾焽閳绘梹銇勯幘璺烘瀾闁绘帟妫勯湁闁稿繑绁归鍫濆偍婵炴垶姘ㄥΛ顖炴煥閻曞倹瀚�
    TIM_Cmd(TIM1,DISABLE); */                                                   //濠电姰鍨洪崕鑲╁垝妤ｅ啫绀傛俊顖氱毞閸嬫挾鎲撮崟顓ф殹濠碉紕鍋涢崐鍨暦閹惰姤鏅搁柨鐕傛嫹
}
void HC_SR04_start(void)
{   
    GPIO_ResetBits(HCSR04_PORT_Trig,HCSR04_TRIG);                           
    delay_us(10);
    GPIO_SetBits(HCSR04_PORT_Trig,HCSR04_TRIG);                             //缂傚倸鍊烽悞锕傛偡閵堝洨鍗氶悗娑欘焽閳绘棃鏌ㄩ悤鍌涘10us闂備焦鐪归崝宀€鈧矮鍗抽幃鐢割敋閳ь剟骞冩禒瀣棃婵炴垯鍨虹亸娆撴⒑閻戔晛浜鹃柛鏂跨Т椤洭妫冨☉鍗炴闂侀€炲苯澧寸€规洏鍎甸、娑樷攽閸╄￥鍨归…鍧楀捶椤撶儐鏆柣蹇撻獜閹凤拷
    delay_us(10);
    GPIO_ResetBits(HCSR04_PORT_Trig,HCSR04_TRIG);
	
}

void TIM1_Init(u16 arr,u16 psc)  
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//濠电偠鎻紞鈧柛搴☆煼瀹曟螣鐏忔牕浜鹃悷娆忓椤ｅ弶淇婇悙鎻掆偓鍨暦閹惰姤鏅搁柨鐕傛嫹1闂備礁鎼崯顐︽偉婵傜ǹ绠伴柨鐕傛嫹
	 
	TIM_TimeBaseStructure.TIM_Period = arr; //闂佽崵濮崇粈浣规櫠娴犲鍋柛鈩冪☉閹瑰爼鏌曟繛鍨偓妤呮偡閹炬枼妲堥柟缁㈠灠娴滅偓绻涢幋鐐存儎濞存粍绻傜叅闁秆勵殔濡ê螞妫颁浇鍏岄柣銊﹀灥闇夐柣妯哄暱閸旇埖銇勯妷銉含鐎规洘顨婃俊鐑芥晜婵劒鎴烽梻浣告啞閺岋綁宕濇惔鈾€鏋旈柟杈鹃檮閸ゅ﹥銇勮箛鎾愁仼鐞氱喖姊绘担鐟扮祷缂佸鍏樿棢濠电姴鍟慨婊堟煢濡警妲烘俊鍙夊灴閹鈽夊▍顓т簻閳诲秹骞掑Δ鈧婵嬫煏婵犲繘妾柛鈺佸€块弻锝夊Ω閵夈儺浠奸梺閫炲苯澧鹃柟鍑ゆ嫹
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //闂佽崵濮崇粈浣规櫠娴犲鍋柛鈩冪⊕閸嬨劑鏌曟繝蹇涙闂傚嫬绉撮湁闁挎繂妫涢惌鍡涙偣閹邦喖鍘矷Mx闂備礁鎼崯顐︽偉婵傜ǹ绠伴柟鎹愭彧缁憋綁鏌熺€电ǹ孝闁肩儤濞婂濠氬礋閳轰讲鍋撻弽顓炴辈闁绘柨鍚嬮崕宥夋煕閺囥劌鐏遍柡鍡樻礋閺屾盯骞掗幋鐑嗘＆濡炪倧绲介悥鐓庣暦閹扮増鏅搁柨鐕傛嫹 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //闂佽崵濮崇粈浣规櫠娴犲鍋柛鈩冪☉缁秹鏌涢锝嗙閻庢鍙冮弻娑㈠箳閹寸儐妫炴繝銏㈡閹凤拷:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM闂備礁鎲＄喊宥夊垂瀹曞洨绠旈柛灞剧矋婵ジ鏌嶉埡浣告殲婵炲牐鍩栨穱濠囶敍濡炶浜剧€规洖娲ㄩ、锟�
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //闂備礁鎼粔鐑斤綖婢跺﹦鏆ゅ☉鏂哄亾IM_TimeBaseInitStruct濠电偞鍨堕幖鈺呭储閻ｅ备鍋撻崹顐ｇ殤闁逞屽墲椤顫濋妸銉㈡灁闁硅揪绠戦惌妤呮煕鐏炲墽鈯曟繛鍫濈埣閺屾盯骞掗幘鍓佺暤濡炪値鍋呴崝娆忕暦閻樿瀚夋い銉ф灩x闂備焦鐪归崝宀€鈧凹鍘奸—鍐磼閻愮补鎷婚梺鍝勵槹閸ㄥ綊宕妸鈺傜厸濞达綀顫夌欢鑼偓瑙勬礈閸犳劙骞忛崨瀛樻櫢闁跨噦鎷�

	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
	//濠电偞鍨堕幖鈺呭储娴犲钃熼柛銉ｅ妼椤曡鲸淇婇姘儓閻㈩垬鍎崇槐鎺旂磼閵忥紕顔嘨IC闂佽崵濮崇粈浣规櫠娴犲鍋柨鐕傛嫹
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1濠电偞鍨堕幖鈺呭储娴犲钃熼柨鐕傛嫹
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //闂備胶顢婄紙浼村垂閻熸壆鏆﹂柣妤€鐗嗛杈ㄤ繆椤栨碍鎯堥悽顖樺劤缁辨帡骞橀崨顔碱伓0缂傚倷鐒﹂幏婵嬪箯閿燂拷
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //濠电偛顕慨瀵歌姳閼测晜顫曟い鎾卞灩缁€鍌炴煕椤愮姴鐏查柣娑虫嫹3缂傚倷鐒﹂幏婵嬪箯閿燂拷
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ闂傚倷绶￠崑鍛暆閸涘﹦顩查柛鎾楀嫬浠忓銈嗗姀閳ь剚鏋奸弸蹇涙⒑閼测晩鍔滈柟鍑ゆ嫹
	NVIC_Init(&NVIC_InitStructure);  //闂備礁鎲＄敮妤冩崲閸岀儑缍栭柟鐗堟緲缁€宀勬煛閸喖鎷籌C闂佽閰ｅ褏鈧凹鍓熼幃娲Ω閳轰線妫烽梺璺ㄥ櫐閹凤拷
	TIM_Cmd(TIM1, ENABLE); 
} 
void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//??
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;		//GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 7200 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//闂備浇娉曢崰鎰板几婵犳艾绠柣鎴ｅГ閺呮悂鏌℃径瀣€滄い鏃€鍔欓獮蹇涙晸閿燂拷
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}
void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2,Compare);
}
void tim3_encode_init(void)
{  
	GPIO_InitTypeDef GPIO_InitTIM3;
	NVIC_InitTypeDef  NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_InternalClockConfig(TIM3);//tim3  
	
	GPIO_InitTIM3.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitTIM3.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitTIM3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitTIM3);
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 65535;		//ARR
	TIM_TimeBaseStructure.TIM_Prescaler = 1-1;		//PSC
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter =10;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter =10;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
	
	TIM_Cmd(TIM3,ENABLE);
}
int16_t Read_Encoder3(void)
{
    int16_t Encoder_TIM;
    Encoder_TIM= TIM3->CNT;
    TIM_SetCounter(TIM3, 0);
   	return Encoder_TIM;
}
void tim4_encode_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitTIM4;
	GPIO_InitTIM4.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitTIM4.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitTIM4.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitTIM4);
	
	TIM_TimeBaseInitTypeDef tim4;
	tim4.TIM_ClockDivision = TIM_CKD_DIV1;
	tim4.TIM_CounterMode = TIM_CounterMode_Up;
	tim4.TIM_Period = 65535;		//ARR
	tim4.TIM_Prescaler = 1-1;		//PSC
	TIM_TimeBaseInit(TIM4, &tim4);
	
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter =10;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter =10;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM4,ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
}
int16_t Read_Encoder4(void)
{
    int16_t Encoder_TIM;
    Encoder_TIM= TIM_GetCounter(TIM4);
    TIM_SetCounter(TIM4, 0);
   	return Encoder_TIM;
}
void Limiting_motor(void)
{
	if(pwm_left>7199)
	{
		pwm_left=7199;
	}
	else if(pwm_left<-7199)
	{
		pwm_left=-7199;
	}
	if(pwm_right>7199)
	{
		pwm_right=7199;
	}
	else if(pwm_right<-7199)
	{
		pwm_right=-7199;
	}
}
void AIN1_set(void)
{
	GPIO_SetBits(GPIOB,AIN1);
}
void AIN1_reset(void)
{
	GPIO_ResetBits(GPIOB,AIN1);
}
void AIN2_set(void)
{
	GPIO_SetBits(GPIOB,AIN2);
}
void AIN2_reset(void)
{
	GPIO_ResetBits(GPIOB,AIN2);
}
void BIN1_set(void)
{
	GPIO_SetBits(GPIOB,BIN1);
}
void BIN1_reset(void)
{
	GPIO_ResetBits(GPIOB,BIN1);
}
void BIN2_set(void)
{
	GPIO_SetBits(GPIOB,BIN2);
}
void BIN2_reset(void)
{
	GPIO_ResetBits(GPIOB,BIN2);
}
void forward(void)
{
	GPIO_SetBits(GPIOB,AIN1);
	GPIO_ResetBits(GPIOB,AIN2);
	GPIO_SetBits(GPIOB,BIN2);
	GPIO_ResetBits(GPIOB,BIN1);
}
void behind(void)
{
	AIN2_set();
	AIN1_reset();
	BIN2_reset();
	BIN1_set();
}