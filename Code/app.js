const sharp = require("sharp");
const fs = require('fs');
const fsp = require('fs').promises;
const Module = require('./ipl.js');




async function testIPLLibrary() {
  try {
    const files = ["../../Data/images/tiger.jpg", "../../Data/images/bird.jpg", "../../Data/images/bird2.jpg"]
    const outputfiles = ["../../Data/lib_output/tiger.jpg", "../../Data/lib_output/bird.jpg", "../../Data/lib_output/bird2.jpg"];
    const outputfilesOriginal = ["../../Data/lib_output/tiger_o.jpg", "../../Data/lib_output/bird_o.jpg", "../../Data/lib_output/bird2_o.jpg"];
    let base64Combined = await combinesBase64Images(files);
    let images = new Module.ImageOperations();
    images.createImages(base64Combined);
    images.grayscale_avgs();
    const base64Response = images.encodeBytes();
    await splitsAndWriteBase64Images(base64Response, outputfiles);
    images.Undos();
    const base64Response2 = images.encodeBytes();
    await splitsAndWriteBase64Images(base64Response2, outputfilesOriginal);
    
    
  } catch (error) {
    console.log(`An error occurred during processing: ${error}`);
  }
}

async function combinesBase64Images(files) {
  let base64Combined = "";
  for (const file of files) {
    const bufferedImage = await sharp(file).toBuffer();
    let base64Str = bufferedImage.toString('base64');
    base64Combined = base64Combined + base64Str + ",";
  }
  return base64Combined;
}

async function splitsAndWriteBase64Images(base64Response, outF) {
  let base64Combined = base64Response.split(",");
  console.log(base64Combined.length);

  for (i = 0; i < base64Combined.length; i++) {
    const buffer = Buffer.from(base64Combined[i], "base64");
    fs.writeFileSync(outF[i], buffer);
  }
}


testIPLLibrary();