const sharp = require("sharp");
const fs = require('fs');
const fsp = require('fs').promises;
const Module = require('./ipl_min.js');

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

async function writeBase64Images(base64Response, filename) {
  let base64Combined = base64Response.split(",");
  const buffer = Buffer.from(base64Combined[0], "base64");
  fs.writeFileSync(filename, buffer);
  
}

function writeMetrics(metrics) {
  fs.appendFile("jsMetrics_min.csv", metrics, (err) => {
    if (err) { console.log(err); }
    console.log("Appended");
  });
}


async function createTestImages(images, filenames){
    images.grayscale_avgs();
    base64Response = images.encodeBytes();
    writeBase64Images(base64Response, (filenames[0] + "_gray1.jpg"));
    images.Undos();
    base64Response = images.encodeBytes();
    writeBase64Images(base64Response, (filenames[0] + "_original.jpg"));
    images.grayscale_lums();
    base64Response = images.encodeBytes();
    writeBase64Images(base64Response, (filenames[0] + "_gray2.jpg"));
    images.Undos();
    images.colorMasks(1, 0, 0);
    base64Response = images.encodeBytes();
    writeBase64Images(base64Response, (filenames[0] + "_mask.jpg"));
    images.Undos();
    images.encodeMessages("Some looooooooooooooooooooooooooooooooooooooooooooooooooooon strinnnnnnnnnnnnnnnnnnnnnnnnnnng");
    const decodedString = images.decodeMessagesLib();
    images.Undos();
    images.std_convolve_clamp_to_0(0, 3, 3, 1, 1);
    base64Response = images.encodeBytes();
    writeBase64Images(base64Response, (filenames[0] + "_blur1.jpg"));
    images.Undos();
    images.std_convolve_clamp_to_border(0, 3, 3, 1, 1);
    base64Response = images.encodeBytes();
    writeBase64Images(base64Response, (filenames[0] + "_blur2.jpg"));
    images.Undos();
    images.flipX();
    base64Response = images.encodeBytes();
    writeBase64Images(base64Response, (filenames[0] + "_flipX.jpg"));
    images.Undos();
    images.flipY();
    base64Response = images.encodeBytes();
    writeBase64Images(base64Response, (filenames[0] + "_flipY.jpg"));
}

async function createMetrics(images, n){

    for (i = 0; i < n; i++){
      images.grayscale_avgs();
      images.Undos();
      images.grayscale_lums();
      images.Undos();
      images.colorMasks(1, 0, 0);
      images.Undos();
      images.encodeMessages("Some looooooooooooooooooooooooooooooooooooooooooooooooooooon strinnnnnnnnnnnnnnnnnnnnnnnnnnng");
      const decodedString = images.decodeMessagesLib();
      images.Undos();
      images.std_convolve_clamp_to_0(0, 3, 3, 1, 1);
      images.Undos();
      images.std_convolve_clamp_to_border(2, 3, 3, 1, 1);
      images.Undos();
      images.flipX();
      images.Undos();
      images.flipY();
      images.Undos();
      const base64Response2 = images.encodeBytes();
      //await splitsAndWriteBase64Images(base64Response2, outputfilesOriginal);

    }

    const metrics = images.getMetrics();
    writeMetrics(metrics);
  
}


async function runOperations(n) {
  try{

    const files = ["../../Data/images/tiger.jpg", "../../Data/images/tiger.jpg"]
    const outfiles = ["../../Data/jsOutput/tiger", "../../Data/jsOutput/tiger"]
    let base64Combined = await combinesBase64Images(files);
    let images1 = new Module.ImageOperations();
    images1.createImages(base64Combined);
    // await createTestImages(images1, outfiles);

    let images2 = new Module.ImageOperations();
    images2.createImages(base64Combined);
    await createMetrics(images2, 50);

  
  }catch (error) {
    console.log(`An error occurred during processing: ${error}`);
  }
    
};


async function testIPLLibrary() {
  await runOperations(50);
}


testIPLLibrary();