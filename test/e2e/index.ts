import chai from "chai";
import chaiHttp from "chai-http";

chai.should();
const SERVER_URL = process.env.APP_URL || "http://localhost:8080";
chai.use(chaiHttp);

/*

For this to work:

  -   have webserv running locally
  -   go to browser and navigate to localhost:8080
      if you skip this step server will shut down when running test suite
  -   run `make test_e2e` from root, always keep checking if local running server shutdown

*/

describe("General", () => {

  describe("GET /", () => {
    it("should return index", done => {
      chai
        .request(SERVER_URL)
        .get("/")
        .set('Connection', 'keep-alive') // trying to match browser request headers
        .set('Accept-Encoding', 'gzip, deflate, br')
        .end((err, res) => {
          if (err) done(err)

          res.should.have.status(200);
          res.should.be.html;
          res.text.should.include('Welcome to our server');
          done();
        });
    });

  });

  describe("GET /forest.jpeg", () => {
    it("should return requested image", done => {
      chai
        .request(SERVER_URL)
        .get("/forest.jpeg")
        .end((err, res) => {
          if (err) done(err)

          // Check the response created by the server on the console
          // console.log({res});

          res.should.have.status(200);
          res.body.should.exist;
          res.should.have.header('content-type', 'image/jpeg');
          res.should.have.header('content-length', '5739587');
          done();
        });
    });

  });

})
