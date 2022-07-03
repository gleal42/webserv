import chai from "chai";
import chaiHttp from "chai-http";

chai.should();
const SERVER_URL = process.env.APP_URL || "http://localhost:8080";
chai.use(chaiHttp);

describe("Hello", () => {

  describe("GET /", () => {
    it("should return epic index", done => {
      chai
        .request(SERVER_URL)
        .get("/")
        .end((err, res) => {
          if (err) done(err)

          // Check the response created by the server on the console
          // console.log(res);

          res.should.have.status(200);
          res.should.be.html;
          res.text.should.include('Like a Boss');
          done();
        });
    });

  });

})
