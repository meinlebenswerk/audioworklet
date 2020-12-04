const { AudioServer } = require('../');
const audioServer = new AudioServer();

const main = async () => {
  const outDevice = audioServer.getDefaultOutputDevice();
  console.log(`Recording from ${outDevice.name}`);

  const CHANNELS = 2;
  const SAMPLE_RATE = 48000;

  const inputStream = audioServer.initInputStream(outDevice.id, {
    sampleRate: SAMPLE_RATE,
    format: AudioServer.F32LE,
    channels: CHANNELS,
  }, true);

  inputStream.registerAudioCallback((data) => console.log(new Float32Array(data).slice(0, 10)))

  inputStream.start();

  setTimeout(() => {
    inputStream.stop();
  }, 10000);
}

main();
