import chai from "chai";
import chaiHttp from "chai-http";

chai.should();
const SERVER_URL = process.env.APP_URL || "http://localhost:8080";
chai.use(chaiHttp);

describe("c10k", () => {

  describe.skip("GET /", () => {
    it("should allow 10k requests without killing the server", done => {
      chai
        .request(SERVER_URL)
        .get("/")
        .end((err, res) => {
          if (err) done(err)

          // Check the response created by the server on the console
          console.log({res});

          res.should.have.status(200);
          res.should.be.html;
          res.text.should.include('Like a Boss');
          done();
        });
    });

  });

})
