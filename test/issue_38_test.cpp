#define BOOST_TEST_MODULE issue_38

#include <airmap/airspace.h>
#include <airmap/codec.h>

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(prevent_regression_for_issue_38) {
  constexpr const char* reply = R"__(
      {"data":[{"id":"94af45f4-0f72-4db3-a094-0fb2d204ef0d","latitude":35.5937866,"longitude":9.2075463,"min_circle_radius":1801211.63710603,"name":"GPS/GNSS Notice: GNSS area wide operations (NOTAM DTTC A842)","type":"notam","country":"TUN","state":"Siliana","city":null,"last_updated":"2017-11-01T08:56:28.000Z","properties":{"body":"GNSS AREA\nREF AIC 5/06\nNOTAM RELATED TO THE AVAILABILITY OF THE RECEIVER AUTONOMOUS \nINTEGRITY MONITORING FUNCTION(RAIM) ARE SUSPENDED","code23":"GW","notam_airmap_type_id":6,"notam_airmap_type_description":"GPS/GNSS Notice","effective_start":"2017-08-11T08:14:00.000Z","effective_end":"2017-11-11T11:00:00.000Z"},"geometry":{"type":"Polygon","coordinates":[[[28.7299722026771,32.4368986947919],[27.7386425808344,29.4165983744077],[26.2084288321399,26.5746039557699],[24.1987240824956,23.985375657354],[21.7656821564908,21.7234604918666],[18.9698044468117,19.8631074430494],[15.8823654481718,18.4742519721294],[12.588871849642,17.6156299357394],[9.18823105414503,17.32686722995],[5.78752335179738,17.6221219785592],[2.49394219912415,18.4876121009481],[-0.593286184109728,19.8839865718145],[-3.38818235660412,21.7526107611843],[-5.81890433216938,24.0234457196175],[-7.8243339375382,26.6219373650009],[-9.34767569751106,29.473050759164],[-10.3288835938873,32.5016720288099],[-10.6978288900042,35.629527871057],[-10.370431051615,38.7694009793464],[-9.25102694187264,41.8179660248007],[-7.24604229701232,44.6494254942697],[-4.29501598907078,47.1135692601557],[-0.420867618630638,49.0434774932022],[4.21401639686308,50.2776950309149],[9.28318819103924,50.6956673940946],[14.3450475555472,50.2532668943004],[18.9601790047435,48.9977488876145],[22.8072979931465,47.0517981688405],[25.7294261358261,44.577639357843],[27.7074885081527,41.7418700676832],[28.8039262202336,38.6937787625542],[29.1129218497452,35.5580431656801],[28.7299722026771,32.4368986947919]]]}},{"id":"8d59f672-2061-40f2-879b-161ecc996bbc","latitude":47.4033337,"longitude":8.7011119,"min_circle_radius":12593.7200854949,"name":"LSMD (DUBENDORF CTR)","type":"controlled_airspace","country":"CHE","state":"Zürich","city":"Illnau-Effretikon","last_updated":"2017-10-31T18:54:51.000Z","properties":{"type":"CTR","airspace_class":"d"},"geometry":{"type":"Polygon","coordinates":[[[8.859167,47.439722],[8.840278,47.400278],[8.802222,47.320278],[8.78,47.273056],[8.605833,47.287222],[8.543056,47.366944],[8.540833,47.396944],[8.699444,47.474167],[8.859167,47.439722]]]}},{"id":"8d59f672-2061-40f2-879b-161ecc996bbc","latitude":47.3070832,"longitude":8.6615286,"min_circle_radius":11885.9921265207,"name":"LSZH (ZURICH CTR 2)","type":"controlled_airspace","country":"CHE","state":"Zürich","city":"Egg","last_updated":"2017-10-31T18:54:51.000Z","properties":{"type":"CTR","airspace_class":"d"},"geometry":{"type":"Polygon","coordinates":[[[8.686389,47.384444],[8.792778,47.248333],[8.666389,47.224444],[8.618889,47.236667],[8.530278,47.365833],[8.661389,47.375278],[8.686389,47.384444]]]}},{"id":"8d59f672-2061-40f2-879b-161ecc996bbc","latitude":47.4298992,"longitude":8.5504361,"min_circle_radius":18094.4196872762,"name":"LSZH (ZURICH CTR 1)","type":"controlled_airspace","country":"CHE","state":"Zürich","city":"Zürich","last_updated":"2017-10-31T18:54:51.000Z","properties":{"type":"CTR","airspace_class":"d"},"geometry":{"type":"Polygon","coordinates":[[[8.758333,47.410556],[8.686389,47.384444],[8.661389,47.375278],[8.530278,47.365833],[8.340278,47.351667],[8.33,47.461389],[8.331032,47.474663],[8.333799,47.487822],[8.338281,47.500762],[8.344442,47.51338],[8.352235,47.525577],[8.3616,47.537256],[8.372462,47.548325],[8.384738,47.558695],[8.398329,47.568285],[8.413129,47.577018],[8.429021,47.584826],[8.445878,47.591645],[8.463567,47.597422],[8.481947,47.60211],[8.500872,47.605674],[8.520192,47.608084],[8.539752,47.609321],[8.559396,47.609375],[8.57897,47.608247],[8.598315,47.605944],[8.617279,47.602487],[8.635711,47.597901],[8.653464,47.592224],[8.670397,47.585501],[8.686376,47.577786],[8.701274,47.56914],[8.714973,47.559631],[8.727366,47.549336],[8.738355,47.538335],[8.747853,47.526717],[8.755787,47.514574],[8.762095,47.502001],[8.766728,47.489099],[8.769651,47.47597],[8.770842,47.462716],[8.770293,47.449444],[8.76801,47.436257],[8.764012,47.42326],[8.758333,47.410556]]]}},{"id":"1c1d06c1-e803-40a6-a530-5a7e5347a54f","latitude":58.4721658,"longitude":-4.8741217,"min_circle_radius":1706549.64290281,"name":"Aerial Activity: Aerial survey (NOTAM EGTT H6885)","type":"notam","country":"GBR","state":"Scotland","city":null,"last_updated":"2017-11-01T08:56:28.000Z","properties":{"body":"AERIAL SURVEY\nPIPELINE INSPECTION NOTIFICATION PROCEDURE SYSTEM (PINS) WILL TAKE\nPLACE IN THE FOLLOWING LOW FLYING AREAS 1, 2, 4, 5, 6, 7S, 8, 9,\n10, 11S, AND THAMES VALLEY AVOIDANCE AREA UK AIP ENR 6-1-10-3\nREFERS 17/10/086/LFBC","code23":"WY","notam_airmap_type_id":8,"notam_airmap_type_description":"Aerial Activity","effective_start":"2017-11-01T08:00:00.000Z","effective_end":"2017-11-01T17:00:00.000Z"},"geometry":{"type":"Polygon","coordinates":[[[22.4805242044634,52.2790246252087],[20.3772798320402,49.4259256458032],[17.7620955020385,46.7968430596868],[14.7244436839284,44.441076739619],[11.3342034941461,42.4080977569206],[7.65385969101926,40.7474817412106],[3.74697945872952,39.5060177964229],[-0.317206366191477,38.7228595842837],[-4.46309820465402,38.4241428111175],[-8.61129729035728,38.6188228408544],[-12.6830735209525,39.2972164579478],[-16.6048027151061,40.4328053432995],[-20.3104695906962,41.9866649335806],[-23.7409229117653,43.913008378792],[-26.8393716972794,46.1641312433511],[-29.5432805472641,48.6934053958478],[-31.7732681364916,51.455512799504],[-33.4196688834963,54.4035163864488],[-34.3259550137461,57.4826291623788],[-34.2641414642256,60.6207477715322],[-32.898525105243,63.715405579238],[-29.766035169586,66.6156142789983],[-24.3648608377876,69.1011116088602],[-16.4650404187122,70.880581188121],[-6.6374347746978,71.6539683034037],[3.51449626982835,71.2570281185448],[12.1758046594634,69.7743743153836],[18.4148221692213,67.4774841466324],[22.2587387189134,64.6707121562289],[24.1576421538668,61.5998515313906],[24.5943126464094,58.4382319012136],[23.9467390899043,55.3038165110541],[22.4805242044634,52.2790246252087]]]}},{"id":"60adaba6-06b0-4a5a-9e24-405ca94a406c","latitude":58.4721658,"longitude":-4.8741217,"min_circle_radius":1706549.64290281,"name":"Aerial Activity: Aerial survey (NOTAM EGPX H6885)","type":"notam","country":"GBR","state":"Scotland","city":null,"last_updated":"2017-11-01T08:56:28.000Z","properties":{"body":"AERIAL SURVEY\nPIPELINE INSPECTION NOTIFICATION PROCEDURE SYSTEM (PINS) WILL TAKE\nPLACE IN THE FOLLOWING LOW FLYING AREAS 1, 2, 4, 5, 6, 7S, 8, 9,\n10, 11S, AND THAMES VALLEY AVOIDANCE AREA UK AIP ENR 6-1-10-3\nREFERS 17/10/086/LFBC","code23":"WY","notam_airmap_type_id":8,"notam_airmap_type_description":"Aerial Activity","effective_start":"2017-11-01T08:00:00.000Z","effective_end":"2017-11-01T17:00:00.000Z"},"geometry":{"type":"Polygon","coordinates":[[[22.4805242044634,52.2790246252087],[20.3772798320402,49.4259256458032],[17.7620955020385,46.7968430596868],[14.7244436839284,44.441076739619],[11.3342034941461,42.4080977569206],[7.65385969101926,40.7474817412106],[3.74697945872952,39.5060177964229],[-0.317206366191477,38.7228595842837],[-4.46309820465402,38.4241428111175],[-8.61129729035728,38.6188228408544],[-12.6830735209525,39.2972164579478],[-16.6048027151061,40.4328053432995],[-20.3104695906962,41.9866649335806],[-23.7409229117653,43.913008378792],[-26.8393716972794,46.1641312433511],[-29.5432805472641,48.6934053958478],[-31.7732681364916,51.455512799504],[-33.4196688834963,54.4035163864488],[-34.3259550137461,57.4826291623788],[-34.2641414642256,60.6207477715322],[-32.898525105243,63.715405579238],[-29.766035169586,66.6156142789983],[-24.3648608377876,69.1011116088602],[-16.4650404187122,70.880581188121],[-6.6374347746978,71.6539683034037],[3.51449626982835,71.2570281185448],[12.1758046594634,69.7743743153836],[18.4148221692213,67.4774841466324],[22.2587387189134,64.6707121562289],[24.1576421538668,61.5998515313906],[24.5943126464094,58.4382319012136],[23.9467390899043,55.3038165110541],[22.4805242044634,52.2790246252087]]]}},{"id":"00207b04-64cf-462f-845f-eebe575b21d1","latitude":47.3985725,"longitude":8.6472377,"min_circle_radius":0,"name":"Dubendorf","type":"airport","country":"CHE","state":"Zürich","city":"Dübendorf","last_updated":"2017-10-05T11:44:15.000Z","properties":{"icao":"LSMD","paved":true,"phone":"+418232485","tower":true,"runways":[{"name":"11","length":2355,"true_bearing":107.76,"runway_threshold_latitude":47.4016111,"runway_threshold_longitude":8.6337083},{"name":"29","length":2355,"true_bearing":287.78,"runway_threshold_latitude":47.3953,"runway_threshold_longitude":8.6627083}],"elevation":1450,"longest_runway":2355,"instrument_approach_procedure":true},"geometry":{"type":"Point","coordinates":[8.64820833333333,47.3984583333333]},"rules":[{"name":"Commercial Rules","geometry":{"type":"Polygon","coordinates":[[[8.56161298233715,47.4028037110359],[8.56151909043402,47.4044843520088],[8.56223917391703,47.4075844042257],[8.56262247209717,47.4105102958028],[8.56327608215597,47.4120477878383],[8.56352197846726,47.4131061405749],[8.56448802412762,47.4148984027311],[8.56615008746993,47.4188072314427],[8.56660853100677,47.4195928500769],[8.56836019828527,47.4223262529668],[8.56976761243906,47.4243373338819],[8.57738069682667,47.4327072632575],[8.58749490411893,47.4397536846604],[8.58955121491959,47.4409027937321],[8.59877399201339,47.44522327117],[8.60086666345508,47.4460354511538],[8.60978844964289,47.4489217448402],[8.61287084424503,47.4497335332853],[8.62547841655674,47.4521134364178],[8.63853001866373,47.452766467108],[8.65151628868936,47.4516671363971],[8.65670891722833,47.4504924655991],[8.66193988250251,47.4497604293348],[8.6741768942253,47.4462055877175],[8.68511887147211,47.441063551208],[8.68864234146362,47.4390236220984],[8.69018199201092,47.437957670704],[8.6910777200465,47.4377586838734],[8.70256860809373,47.4333350973497],[8.70355149234297,47.4327551670545],[8.70414377594271,47.4325167883957],[8.7044676485847,47.4323649621076],[8.71436954169051,47.4266749634344],[8.72244654111978,47.4197846023788],[8.72838957182668,47.4119581926149],[8.7319717346713,47.4034958324655],[8.73305688654787,47.3947218589424],[8.73160469405497,47.3859723915391],[8.72767199171072,47.3775824479533],[8.72726105608633,47.3769334933631],[8.72676271183226,47.376126278128],[8.72659666236347,47.3758573969151],[8.72000914382117,47.3676833538172],[8.71108638085743,47.3606059889437],[8.70021141261112,47.3549284510745],[8.687850428455,47.3508938425269],[8.67453283126942,47.3486748704212],[8.66082867986007,47.3483665026653],[8.65696734735344,47.3488285834393],[8.64793730423919,47.3498666086846],[8.64673415694893,47.3498279294437],[8.64548036382133,47.3498688842294],[8.64209317692636,47.3500387369618],[8.64166952751379,47.3500674165268],[8.63794928896087,47.3503917622031],[8.6371341081008,47.3504788332156],[8.63037005373738,47.3516111755507],[8.62860816589359,47.3517814124417],[8.62853987818967,47.3517810446674],[8.62830406361566,47.3518107925176],[8.62615912956572,47.3520179960624],[8.6228303339711,47.352432619343],[8.6228482077446,47.3524989001333],[8.61609610368391,47.3533501094243],[8.60431784504473,47.3564984960826],[8.59363470156856,47.3611114060628],[8.58443660478473,47.3670206063708],[8.57775308176453,47.3722211091578],[8.57027993185705,47.379321493593],[8.56496404480171,47.3872681435558],[8.56294746178931,47.3930597169287],[8.56289003170333,47.3930518256203],[8.56281030581295,47.393319371333],[8.56277367289833,47.3935587720983],[8.56200629524177,47.3957622694461],[8.56175907977955,47.4001883771565],[8.56149576120771,47.4019086856813],[8.56161298233715,47.4028037110359]]]},"min_circle_radius":6520.2957739684},{"name":"Recreational Rules","geometry":{"type":"Polygon","coordinates":[[[8.56161298233715,47.4028037110359],[8.56151909043402,47.4044843520088],[8.56223917391703,47.4075844042257],[8.56262247209717,47.4105102958028],[8.56327608215597,47.4120477878383],[8.56352197846726,47.4131061405749],[8.56448802412762,47.4148984027311],[8.56615008746993,47.4188072314427],[8.56660853100677,47.4195928500769],[8.56836019828527,47.4223262529668],[8.56976761243906,47.4243373338819],[8.57738069682667,47.4327072632575],[8.58749490411893,47.4397536846604],[8.58955121491959,47.4409027937321],[8.59877399201339,47.44522327117],[8.60086666345508,47.4460354511538],[8.60978844964289,47.4489217448402],[8.61287084424503,47.4497335332853],[8.62547841655674,47.4521134364178],[8.63853001866373,47.452766467108],[8.65151628868936,47.4516671363971],[8.65670891722833,47.4504924655991],[8.66193988250251,47.4497604293348],[8.6741768942253,47.4462055877175],[8.68511887147211,47.441063551208],[8.68864234146362,47.4390236220984],[8.69018199201092,47.437957670704],[8.6910777200465,47.4377586838734],[8.70256860809373,47.4333350973497],[8.70355149234297,47.4327551670545],[8.70414377594271,47.4325167883957],[8.7044676485847,47.4323649621076],[8.71436954169051,47.4266749634344],[8.72244654111978,47.4197846023788],[8.72838957182668,47.4119581926149],[8.7319717346713,47.4034958324655],[8.73305688654787,47.3947218589424],[8.73160469405497,47.3859723915391],[8.72767199171072,47.3775824479533],[8.72726105608633,47.3769334933631],[8.72676271183226,47.376126278128],[8.72659666236347,47.3758573969151],[8.72000914382117,47.3676833538172],[8.71108638085743,47.3606059889437],[8.70021141261112,47.3549284510745],[8.687850428455,47.3508938425269],[8.67453283126942,47.3486748704212],[8.66082867986007,47.3483665026653],[8.65696734735344,47.3488285834393],[8.64793730423919,47.3498666086846],[8.64673415694893,47.3498279294437],[8.64548036382133,47.3498688842294],[8.64209317692636,47.3500387369618],[8.64166952751379,47.3500674165268],[8.63794928896087,47.3503917622031],[8.6371341081008,47.3504788332156],[8.63037005373738,47.3516111755507],[8.62860816589359,47.3517814124417],[8.62853987818967,47.3517810446674],[8.62830406361566,47.3518107925176],[8.62615912956572,47.3520179960624],[8.6228303339711,47.352432619343],[8.6228482077446,47.3524989001333],[8.61609610368391,47.3533501094243],[8.60431784504473,47.3564984960826],[8.59363470156856,47.3611114060628],[8.58443660478473,47.3670206063708],[8.57775308176453,47.3722211091578],[8.57027993185705,47.379321493593],[8.56496404480171,47.3872681435558],[8.56294746178931,47.3930597169287],[8.56289003170333,47.3930518256203],[8.56281030581295,47.393319371333],[8.56277367289833,47.3935587720983],[8.56200629524177,47.3957622694461],[8.56175907977955,47.4001883771565],[8.56149576120771,47.4019086856813],[8.56161298233715,47.4028037110359]]]},"min_circle_radius":6520.2957739684}],"related_geometry":{"property_boundary":{"id":"00207b04-64cf-462f-845f-eebe575b21d1","geometry":{"type":"Polygon","coordinates":[[[8.62766783387077,47.401801406419],[8.62783887310087,47.4019563923342],[8.62775946202975,47.4022239836552],[8.62821868080299,47.4030093602855],[8.62962786798919,47.4050197178929],[8.63168375234843,47.4061680770658],[8.63377538965597,47.4069796590974],[8.63685571276522,47.4077908030537],[8.63889488846017,47.4068063880827],[8.64076122829446,47.4056793830148],[8.64210124520378,47.4068318012005],[8.64562407842198,47.4047931802966],[8.64523456891479,47.4044663832018],[8.64617285922905,47.4023696048095],[8.64729880760616,47.3999169138943],[8.64772371073555,47.3996555129859],[8.6484109219279,47.3993939891697],[8.65137365556646,47.3987040854238],[8.66277237820665,47.3963128842834],[8.66294099198548,47.3968075636486],[8.66428882423777,47.3966064695978],[8.6641880332629,47.3965749706718],[8.66413215805222,47.3965227967337],[8.66391934716141,47.3960759108513],[8.66631829812766,47.3955510628971],[8.66648259999312,47.3954949961051],[8.66680635282152,47.3953432749319],[8.66666747327859,47.3951379793745],[8.66627688579306,47.3945056080858],[8.66566270763331,47.3935089040154],[8.66550002273535,47.3932449779992],[8.66537937899269,47.3932882157389],[8.6650002899428,47.3934698016959],[8.66446561268569,47.3938175863149],[8.66423600329907,47.3939668794103],[8.66412308506786,47.3937288616232],[8.66391521491111,47.3937494769007],[8.6638058001095,47.3937288616232],[8.66362856250395,47.3936538803245],[8.66355642778663,47.393533168115],[8.66348016081901,47.3935643647982],[8.66293901569186,47.3935633918025],[8.66253378566719,47.3936979690978],[8.66300126894104,47.3942857753429],[8.66302031322507,47.3943114985577],[8.66172521207995,47.3945644124289],[8.66119251111647,47.394650460031],[8.66063600479796,47.3947160750338],[8.66005865756486,47.3947664872464],[8.65937162603556,47.3948120953586],[8.65491490424799,47.3951180943654],[8.65125130502476,47.3953663827941],[8.65001028245975,47.3954504831649],[8.64997255321782,47.3951784790946],[8.64991542036575,47.3947664872464],[8.64866056374536,47.3948074737384],[8.64823655893126,47.3948361764259],[8.64742070899022,47.3949233181761],[8.64746957734168,47.3951857155278],[8.64755024605419,47.3956188051236],[8.6459207021288,47.3957303910967],[8.64056925831824,47.3961009642899],[8.6405207492929,47.3961038831364],[8.63810544898848,47.3962705003529],[8.6347739569258,47.3966854600649],[8.63435022160628,47.3965980171852],[8.62766783387077,47.401801406419]]]},"min_circle_radius":1520.29577396842}}},{"id":"1795762e-ab74-4e4a-bbdc-5effafd25566","latitude":47.3757171,"longitude":8.5315697,"min_circle_radius":124.9513925723,"name":"Polizeigefängnis","type":"prison","country":"CHE","state":"Zürich","city":"Zürich","last_updated":"2017-10-05T23:07:59.000Z","properties":{},"geometry":{"type":"Polygon","coordinates":[[[8.53038060022222,47.3749674865719],[8.53020562486826,47.3750949916781],[8.53007012361993,47.3752430215301],[8.52997918188146,47.3754060208989],[8.5299362129429,47.3755778727304],[8.52994282983007,47.3757521276951],[8.52999878470199,47.3759222462148],[8.53010197808032,47.3760818438862],[8.53024853757499,47.3762249310892],[8.53035184338415,47.3763074213486],[8.53054694782255,47.3764343361786],[8.5307758998595,47.3765317256601],[8.530903794475,47.3765639490064],[8.53093635391913,47.3765848173114],[8.53115764485114,47.3766788477605],[8.53140178465542,47.3767415769498],[8.53165929357582,47.3767705691372],[8.5319201726638,47.3767646985671],[8.53217429208803,47.3767241931895],[8.53241178453004,47.376650625808],[8.53262342837925,47.3765468529995],[8.53275745740838,47.3764663703368],[8.53293428609262,47.3763370898615],[8.53307056752161,47.3761868522131],[8.53316106019495,47.3760214360574],[8.53320228391669,47.3758472038232],[8.53319265359472,47.375670856971],[8.53313254013503,47.3754991782294],[8.53302425609956,47.3753387707163],[8.53287196669098,47.3751958039778],[8.53248883397945,47.3749018018277],[8.53229863253654,47.3747818946425],[8.53207766828258,47.3746894766048],[8.53183442071792,47.3746280941977],[8.53157822433308,47.3746001029211],[8.53131891044566,47.3746065769138],[8.5310664299857,47.3746472677405],[8.53083047169456,47.3747206139241],[8.53062009037786,47.374823800858],[8.53038060022222,47.3749674865719]]]}},{"id":"d22eeb0e-9a0a-4ea3-acc4-0e039e7e334c","latitude":47.4650321,"longitude":8.549635,"min_circle_radius":0,"name":"Reference Point","type":"airport","country":"CHE","state":"Zürich","city":"","last_updated":"2017-10-21T13:48:27.000Z","properties":{"ceil":400,"floor":0},"geometry":{"type":"Point","coordinates":[8.54805555555556,47.4580555555556]},"rules":[{"name":"Commercial Rules","geometry":{"type":"Polygon","coordinates":[[[8.46137203073375,47.4799991922499],[8.46218228078076,47.4900911541933],[8.46333781071705,47.4928198379815],[8.46406671817479,47.4966310477889],[8.46892720469395,47.5057812365198],[8.46940264846214,47.5064416470761],[8.47627415886054,47.5139906914782],[8.4769109126465,47.5145565027317],[8.48544149563245,47.5208070328574],[8.49555936992817,47.5258562792063],[8.50690195475786,47.5295230978987],[8.51906244160692,47.5316759064587],[8.51923550915162,47.5316949231964],[8.53353197520959,47.5322064426312],[8.54765769870845,47.5306297840358],[8.55507512077649,47.5286266839706],[8.56055462612873,47.5273337251418],[8.57128838040945,47.5231511730443],[8.5806874542925,47.517690903052],[8.58948668280017,47.5115238972497],[8.591786991633,47.5095568318753],[8.59282836746795,47.5089550875921],[8.59770259566143,47.505393043014],[8.60369945116,47.5014009807662],[8.60691018507548,47.4991075250544],[8.6075775952578,47.4985958899042],[8.61659194279543,47.4948942265399],[8.62661939650487,47.4885356564582],[8.63449956789294,47.4809140760129],[8.63633242440739,47.4786802438786],[8.64172748883176,47.4701234173692],[8.64440946350095,47.4609969790244],[8.64426496041928,47.4516915940372],[8.64130163472504,47.4426053838241],[8.6396802503491,47.4393004791086],[8.63440033518162,47.4312616934289],[8.63298454347409,47.4299019995723],[8.63210898318859,47.4281408974727],[8.62662121676957,47.419927898899],[8.61883221931758,47.4126221873729],[8.60905535729675,47.4065168630648],[8.60465560209193,47.4047141390952],[8.59949971219627,47.4018719035831],[8.58953634503428,47.3980912827002],[8.5832277905834,47.396589208127],[8.58197169302373,47.3961487189974],[8.57917086833223,47.3956230215393],[8.57876638119361,47.3955266800786],[8.57849313816844,47.3954958039352],[8.56877195056729,47.3936705192273],[8.55510441031285,47.3930863754116],[8.54155154974872,47.3944211779736],[8.52869099258145,47.3976180342159],[8.51707106725001,47.4025406762794],[8.50718756966227,47.4089792302217],[8.49535677060472,47.4184627057819],[8.48754629944361,47.4262455903941],[8.48752428731719,47.4262821097324],[8.4815694443049,47.4309431151862],[8.47536692664152,47.4381493033082],[8.47123124625097,47.4460085928457],[8.46984185168045,47.4496988985798],[8.46789048561575,47.4582208236784],[8.46811860526014,47.4623013681738],[8.46394751277398,47.4691379659266],[8.46145826791997,47.4789957704265],[8.46137203073375,47.4799991922499]]]},"min_circle_radius":5000,"requirements":{"notice":{"digital":true,"phone":null}}},{"name":"Recreational Rules","geometry":{"type":"Polygon","coordinates":[[[8.46137203073375,47.4799991922499],[8.46218228078076,47.4900911541933],[8.46333781071705,47.4928198379815],[8.46406671817479,47.4966310477889],[8.46892720469395,47.5057812365198],[8.46940264846214,47.5064416470761],[8.47627415886054,47.5139906914782],[8.4769109126465,47.5145565027317],[8.48544149563245,47.5208070328574],[8.49555936992817,47.5258562792063],[8.50690195475786,47.5295230978987],[8.51906244160692,47.5316759064587],[8.51923550915162,47.5316949231964],[8.53353197520959,47.5322064426312],[8.54765769870845,47.5306297840358],[8.55507512077649,47.5286266839706],[8.56055462612873,47.5273337251418],[8.57128838040945,47.5231511730443],[8.5806874542925,47.517690903052],[8.58948668280017,47.5115238972497],[8.591786991633,47.5095568318753],[8.59282836746795,47.5089550875921],[8.59770259566143,47.505393043014],[8.60369945116,47.5014009807662],[8.60691018507548,47.4991075250544],[8.6075775952578,47.4985958899042],[8.61659194279543,47.4948942265399],[8.62661939650487,47.4885356564582],[8.63449956789294,47.4809140760129],[8.63633242440739,47.4786802438786],[8.64172748883176,47.4701234173692],[8.64440946350095,47.4609969790244],[8.64426496041928,47.4516915940372],[8.64130163472504,47.4426053838241],[8.6396802503491,47.4393004791086],[8.63440033518162,47.4312616934289],[8.63298454347409,47.4299019995723],[8.63210898318859,47.4281408974727],[8.62662121676957,47.419927898899],[8.61883221931758,47.4126221873729],[8.60905535729675,47.4065168630648],[8.60465560209193,47.4047141390952],[8.59949971219627,47.4018719035831],[8.58953634503428,47.3980912827002],[8.5832277905834,47.396589208127],[8.58197169302373,47.3961487189974],[8.57917086833223,47.3956230215393],[8.57876638119361,47.3955266800786],[8.57849313816844,47.3954958039352],[8.56877195056729,47.3936705192273],[8.55510441031285,47.3930863754116],[8.54155154974872,47.3944211779736],[8.52869099258145,47.3976180342159],[8.51707106725001,47.4025406762794],[8.50718756966227,47.4089792302217],[8.49535677060472,47.4184627057819],[8.48754629944361,47.4262455903941],[8.48752428731719,47.4262821097324],[8.4815694443049,47.4309431151862],[8.47536692664152,47.4381493033082],[8.47123124625097,47.4460085928457],[8.46984185168045,47.4496988985798],[8.46789048561575,47.4582208236784],[8.46811860526014,47.4623013681738],[8.46394751277398,47.4691379659266],[8.46145826791997,47.4789957704265],[8.46137203073375,47.4799991922499]]]},"min_circle_radius":5000,"requirements":{"notice":{"digital":true,"phone":null}}}],"related_geometry":{"property_boundary":{"id":"d22eeb0e-9a0a-4ea3-acc4-0e039e7e334c","geometry":{"type":"Polygon","coordinates":[[[8.52762578444493,47.4825968908812],[8.52917034774445,47.4851230703244],[8.52857880712986,47.486041308754],[8.52905482439891,47.4867014762068],[8.52969181976688,47.4872669978835],[8.52986474545907,47.4872859982378],[8.53010567361827,47.4871475929443],[8.53015966236685,47.4871075888902],[8.53191757554634,47.4858202798016],[8.53296932308099,47.4864110642548],[8.54176886027811,47.4802476875382],[8.54264795161515,47.4787366016069],[8.5441670027606,47.4783553777961],[8.55009642262496,47.4740249935862],[8.5571997609026,47.4692991785613],[8.56759219076152,47.461336463672],[8.56918202915136,47.4596708078846],[8.57169192205519,47.4578140177708],[8.5736702818054,47.4574458831663],[8.57468142548921,47.4577328716746],[8.57652791255572,47.4590294935608],[8.57836245202895,47.4567965848661],[8.57674458620225,47.4534908094055],[8.57363713397142,47.4533329987431],[8.57061996242665,47.4518911777832],[8.56899607788755,47.452384363238],[8.56612119948378,47.4521410816697],[8.56425485964949,47.4509686977832],[8.56467311524578,47.4494485082598],[8.5678542293299,47.4484352874115],[8.5677113073682,47.4480721900887],[8.56835863336193,47.4475717957105],[8.56990661025953,47.4461960966965],[8.5701461909458,47.444267080699],[8.56927239966894,47.442505315511],[8.56870987463803,47.4427449963239],[8.56765767794574,47.4426293178118],[8.56448500802528,47.4411327041291],[8.56494710140743,47.4408492097278],[8.56228772884033,47.4391041605057],[8.55990000681514,47.4389760789889],[8.55779498460986,47.4380392151874],[8.5459666876008,47.4475278735292],[8.54873978688288,47.4527391696201],[8.54764186594263,47.4545333926601],[8.54418604704462,47.4552195887347],[8.54365675967922,47.4547040769498],[8.54182752026616,47.4553453832906],[8.54175421773898,47.4556923949116],[8.53550248235068,47.457122996445],[8.53411728018256,47.4608140881147],[8.54392239150873,47.4601475825709],[8.53535866207369,47.4724768102698],[8.53331580328607,47.4724932656878],[8.532978845223,47.4750997145665],[8.52771076507081,47.4815934153743],[8.52762578444493,47.4825968908812]]]},"min_circle_radius":2865.42242095823}}}]}
    )__";

  std::vector<airmap::Airspace> airspaces = nlohmann::json::parse(reply)["data"];

  unsigned int related_geometries_count = 0;

  for (const auto& airspace : airspaces) {
    for (const auto& pair : airspace.related_geometries()) {
      if (pair.first == "property_boundary" && (pair.second.id == "d22eeb0e-9a0a-4ea3-acc4-0e039e7e334c" ||
                                                pair.second.id == "00207b04-64cf-462f-845f-eebe575b21d1")) {
        related_geometries_count++;
      }
    }
  }

  BOOST_CHECK(related_geometries_count == 2);
}
