var path = require("path");
var glob = require('glob');
var fs = require('fs');

createFile();

function randomIntFromInterval(min, max) { // min and max included 
  return Math.floor(Math.random() * (max - min + 1) + min)
}

function createFile() {  
    fs.writeFileSync('../output/fxmanifest.lua', `fx_version 'bodacious'
    games {'gta5'}

    data_file 'INTERIOR_PROXY_ORDER_FILE' 'interiorproxies.meta'
    
    files {
        "interiorproxies.meta"
    }
    `);
    fs.writeFileSync('../output/interiorproxies.meta', `<?xml version="1.0" encoding="UTF-8"?>\n\n<SInteriorOrderData>
    <startFrom value="`+ randomIntFromInterval(2000, 3000) +`" />
    <filePathHash value="0" />
    <proxies>
    `);

    appendYTD()
}

function appendYTD() {
    var OldScriptName = ""
    var ScriptName = ""
    glob("../input/**/**.ymap", async function (er, files) {
        for (i=0; i < files.length; i++) {
            var filePathData = files[i].split(/[\\\/]/)
            var WaitToWrite = false
            var Found = false
            for (var i2 = filePathData.length; i2 >= 1; i2--) {
                if (filePathData[i2] == "stream" ) {
                    Found = true
                } else if (Found) {
                   ScriptName = filePathData[i2]
                    break;
                }
            }
            var name = path.basename(files[i])
            var name2 = name.replace('.ymap', "")
			
            if (OldScriptName != ScriptName ) {
                OldScriptName = ScriptName
                console.log(ScriptName);
                WaitToWrite = true
                fs.appendFile('../output/interiorproxies.meta', (i == 0 ? "  ".repeat(2) : "  ".repeat(4)) + '<!-- ' + ScriptName + ' -->\n', function (err) {
                    WaitToWrite = false
                    if (err) {
                        console.log('An error occured.');
                    } else {
                        return;
                    }
                });
                while (WaitToWrite) {
                    await Delay(0)
                }
            }

            WaitToWrite = true
            fs.appendFile('../output/interiorproxies.meta', (i == 0 ? "  ".repeat(2) : "  ".repeat(4)) + '<Item>' + name2 + '</Item>\n', function (err) {
                WaitToWrite = false
                if (err) {
                    console.log('An error occured.');
                } else {
                    return;
                }
            });
            while (WaitToWrite) {
                await Delay(0)
            }

            if (i == files.length-1) {
                setTimeout(() => {
                    fs.appendFile('../output/interiorproxies.meta', "  ".repeat(2) + '</proxies>\n</SInteriorOrderData>', function(err) {
                        if (err) {
                            console.log('An error occured.');
                        } else {
                            return;
                        }
                    });
                }, 1000);
            }

            console.log(name2)
        }

        console.log('\nA resource has been created at ../output/interiorproxies.meta')

        setTimeout(() => {
            process.exit();
        }, 2000);
    })
}

function Delay(ms) {
    return new Promise((res) => {
        setTimeout(res, ms)
    })
}
