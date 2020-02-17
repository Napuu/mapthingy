const test = require('zora').test;
const rewire = require('rewire');
const fs = require('fs');

const app = rewire('./stylesheet-processor.js');
const main = app.__get__('main');
test('main function should output out.js', async t => {
  await main('./mockdata/in.json', 'out.json');
  t.ok(fs.existsSync('./out.json'), 'should be ok');
  fs.existsSync('./out.json') && fs.unlinkSync('./out.json');
});

test('out.js should equal mockdata/out.js', async t => {
  fs.existsSync('./out.json') && fs.unlinkSync('./out.json');

  await main('./mockdata/in.json', 'out.json');
  await app.__get__('main', './mockdata/in.json', 'out.json');

  const properFileStringified = JSON.stringify(fs.readFileSync('./mockdata/out.json').toJSON());
  const testFileStringified = JSON.stringify(fs.readFileSync('./out.json'));
  t.equal(testFileStringified, properFileStringified, 'out.json should be equal with ./mockdata/out.json');

  fs.unlinkSync('./out.json');
});
