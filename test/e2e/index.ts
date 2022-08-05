import chai from "chai";
import chaiHttp from "chai-http";

chai.should();
const SERVER_URL = process.env.APP_URL || "http://localhost:8080";
chai.use(chaiHttp);

/*

Currently not working due to:

  1) General
       GET /
         should return index:
     Error: Parse Error: Invalid response status
      at Socket.socketOnData (_http_client.js:509:22)
      at addChunk (internal/streams/readable.js:309:12)
      at readableAddChunk (internal/streams/readable.js:284:9)
      at Socket.Readable.push (internal/streams/readable.js:223:10)
      at TCP.onStreamRead (internal/stream_base_commons.js:188:23)

*/

describe.skip("General", () => {

  describe("GET /", () => {
    it("should return index", done => {
      chai
        .request(SERVER_URL)
        .get("/")
        .set('Connection', 'keep-alive') // trying to match browser request headers
        .set('Accept-Encoding', 'gzip, deflate, br')
        .end((err, res) => {
          if (err) done(err)

          // Check the response created by the server on the console
          console.log({res});

          res.should.have.status(200);
          res.should.be.html;
          res.text.should.include('Welcome to our server');
          done();
        });
    });

  });

  describe.skip("GET /test/forest.jpeg", () => {
    it("should return requested image", done => {
      chai
        .request(SERVER_URL)
        .get("/test/forest.jpeg")
        .end((err, res) => {
          if (err) done(err)

          // Check the response created by the server on the console
          console.log({res});

          res.should.have.status(200);
          done();
        });
    });

  });

})
