const db1 = [
    [ "ACRE", "LOREM", "IPSUM", "DOLAM", "SITAM" ],
    [ "ALSO", "ETJCN", "CONZE", "CTEUR", "ADIPS" ],
    [ "CAMP", "LPDSO", "UZQRH", "OQPLK", "TAJCH" ],
    [ "CENT", "LPBSO", "UZQRH", "OQPLK", "FADCH" ],
    [ "CHIP", "NAMES", "AEOUV", "BWSOK", "ERSTF" ],
    [ "DENT", "AEHWN", "ZABLV", "JPMIC", "AHLVQ" ],
    [ "DOME", "RUMAN", "ACFHU", "NIJSK", "FLSCD" ],
    [ "FIRE", "TIHGJ", "GAPUD", "VEYCL", "PTKSN" ],
    [ "ICED", "NULAM", "EOLAB", "OPISN", "ISETA" ],
    [ "HIGH", "STWAU", "DHOFL", "FHIDK", "OEBAN" ],
    [ "HEAR", "YNAFJ", "KRONZ", "CLIRU", "HMAET" ],
    [ "HELP", "FNIRT", "OSJDW", "HKMER", "UWEBT" ],
    [ "KEPT", "ERMCI", "LUYAT", "BECAF", "SHEMW" ],
    [ "KNEW", "ELPTW", "CULPA", "MOLNT", "LAPOR" ],
    [ "KNOB", "OFQSD", "RIPED", "FINTS", "ZLURH" ],
    [ "LAMB", "KODER", "TUVWB", "SNAKY", "CDEFG" ],
    [ "LEAP", "MOREP", "YTOKV", "BIREN", "CODAN" ],
    [ "MELT", "SNACK", "LUNCH", "BFAST", "DINER" ],
    [ "MINE", "SLXOU", "UARYC", "AMPIJ", "LPRAD" ],
    [ "MINT", "GOCUJ", "ALDBO", "ERLVI", "QBFSL" ],
    [ "NEXT", "TOADB", "IPSAK", "JVLCR", "BURKY" ],
    [ "OAKS", "CYABP", "SUEKQ", "JBIMC", "ZCLGE" ],
    [ "OPEN", "TEDPI", "BAWIJ", "SCRIL", "UPBKF" ],
    [ "PALM", "TRIWO", "ECUWF", "JBDYI", "KUFPS" ],
    [ "PASS", "JIMBW", "OIKWD", "NLOTE", "ZKOPY" ],
    [ "PREY", "SHOEL", "HXFBI", "AUOPT", "VFNWJ" ],
    [ "QUIZ", "PIYUM", "ZAMIL", "GKUFS", "FCLWG" ],
    [ "RAMP", "LEHUF", "HIYGR", "LNOCV", "IDCOA" ],
    [ "TENT", "BIGXL", "TINYO", "AMCLB", "DINOB" ],
    [ "XRAY", "YZQCB", "OHSAK", "UNDEC", "ABOKE" ]
];

const db2 = [
  [ "BACK", "PJMET", "ELYHR", "SNPIB", "GCDWV" ],
  [ "BOMB", "DLKFA", "ZGICH", "LADNS", "FRPTJ" ],
  [ "CUBE", "INGEL", "ITSED", "DOEIU", "MAGNL" ],
  [ "EZRA", "SDKUA", "WAJOK", "TSMEX", "YFPMB" ],
  [ "FISH", "LIQUA", "UTEND", "MADIN", "VENIA" ],
  [ "FEET", "MQUIS", "NOSTR", "UDSXR", "CIPAO" ],
  [ "GAME", "JOQDT", "EUSPR", "NBSPX", "ABCFT" ],
  [ "GOLF", "PIELX", "GCHIR", "BMAID", "WATER" ],
  [ "GONE", "HSYOK", "EMTZS", "BIWPG", "ASCIT" ],
  [ "HALO", "IQUPE", "EXRCM", "ODCNS", "UATDS" ],
  [ "HERE", "DUNSA", "UTARD", "OLAND", "LPKHI" ],
  [ "JUMP", "DIREC", "INVOL", "UPTAE", "VELIT" ],
  [ "LAMP", "BZTNO", "SURKW", "GOBSC", "RFTSE" ],
  [ "NAIL", "STMLO", "WPEDZ", "UXFDJ", "MWIFX" ],
  [ "NONE", "QUFWB", "RTANB", "BGRXD", "SPNIF" ],
  [ "NOPE", "JRPHE", "AXRFL", "AYTRM", "PUJMF" ],
  [ "PAIL", "EBIRJ", "NPOSV", "UYLSV", "XFBKI" ],
  [ "PATH", "HRJSX", "QDIGO", "SCRJG", "ZLUVJ" ],
  [ "POND", "EYTHJ", "FWLJE", "CBQRV", "ZXVGP" ],
  [ "PRAY", "BUJDH", "TMUWY", "FWHCE", "BVKCL" ],
  [ "RACE", "XNDFG", "QUOSM", "DUFGZ", "WZOFU" ],
  [ "RAKE", "NVXUA", "ZSQFP", "SXDYJ", "ILBGY" ],
  [ "SEND", "LGOAT", "DOIBQ", "UVRAY", "BIPMO" ],
  [ "SHIP", "FOUNT", "XLEKG", "BLAMP", "SHADE" ],
  [ "SIDE", "MITEY", "CWONG", "WIRTY", "LOQPS" ],
  [ "TICK", "FQHAL", "AQKXL", "XOYID", "GEUZT" ],
  [ "TIME", "ILMUN", "VOCZU", "KDVER", "RCQZO" ],
  [ "TOCK", "SLJGF", "URPVB", "WDXPS", "VJTNW" ],
  [ "WALK", "OFKGT", "SPGHK", "MBNUZ", "DIYLP" ],
  [ "WINK", "CIGPF", "AXRPE", "BADWH", "QHDIP" ]
];


function makePermutations(chars1, chars2, chars3, chars4) {
  const permutations = [];

  for (let i = 0; i < chars1.length; i++) {
    for (let j = 0; j < chars2.length; j++) {
      for (let k = 0; k < chars3.length; k++) {
        for (let l = 0; l < chars4.length; l++) {
          permutations.push(`${chars1.charAt(i)}${chars2.charAt(j)}${chars3.charAt(k)}${chars4.charAt(l)}`);
        }
      }
    }
  }
  return permutations;
}

function hasDuplicateCharacter(s) {
  for (let i = 0; i < s.length; i++) {
    for (let j = i+1; j < s.length; j++) {
      if (s[i] === s[j]) {
        return true;
      }
    }
  }
  return false;
}

function test(db) {
  let errs = false;
  const keys = db.map(row => row[0]);

  for (var i = 0; i < db.length; i++)
  {
    const row = db[i];
    const key = row[0];

    const chars1 = `${key.charAt(0)}${row[1]}`;
    const chars2 = `${key.charAt(1)}${row[2]}`;
    const chars3 = `${key.charAt(2)}${row[3]}`;
    const chars4 = `${key.charAt(3)}${row[4]}`;

    if (hasDuplicateCharacter(chars1)) {
      console.log(`${key} has duplicate characters in ${chars1}`);
      errs = true;
    }

    if (hasDuplicateCharacter(chars2)) {
      console.log(`${key} has duplicate characters in ${chars2}`);
      errs = true;
    }

    if (hasDuplicateCharacter(chars3)) {
      console.log(`${key} has duplicate characters in ${chars3}`);
      errs = true;
    }

    if (hasDuplicateCharacter(chars4)) {
      console.log(`${key} has duplicate characters in ${chars4}`);
      errs = true;
    }

    const perms = makePermutations(chars1, chars2, chars3, chars4);

    const conflicts = perms.filter(perm => keys.includes(perm) && perm !== key);

    if (conflicts.length > 0) {
      console.log(`${key} has conflicts: ${conflicts.join(', ')}`);
      errs = true;
    }
  }
  return !errs;
}


function print() {
  console.log("#include <Arduino.h>");
  console.log();
  console.log("// This file is generated by `node ./src/password-cross-check.js > ./src/passwords.h`");
  console.log();
  format(db1, 'db1');
  format(db2, 'db2');
}
function format(db, varname) {
  for (let r = 0; r < db.length; r++) {
    for (let i = 0; i < db[r].length; i++) {
      console.log(`const char ${varname}_${r.toString().padStart(2, 0)}_${i}[] PROGMEM = "${db[r][i]}";`);
    }
  };
  console.log();
  console.log(`PGM_P const ${varname}[] PROGMEM = {`);
  for (let r = 0; r < db.length; r++) {
    console.log(`  ${varname}_${r.toString().padStart(2, 0)}_${0}, ${varname}_${r.toString().padStart(2, 0)}_${1}, ${varname}_${r.toString().padStart(2, 0)}_${2}, ${varname}_${r.toString().padStart(2, 0)}_${3}, ${varname}_${r.toString().padStart(2, 0)}_${4},`);
  };
  console.log('};')
  console.log();
}
function docs(db) {
  for (let r = 0; r < db.length; r++) {
    console.log(`${db[r][0]}`);
   
  };
}

if (test(db1) && test(db2)) {
  print();
}

// docs(db1);
// docs(db2);


