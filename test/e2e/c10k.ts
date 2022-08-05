import chai from "chai";
import chaiHttp from "chai-http";

chai.should();
const SERVER_URL = process.env.APP_URL || "http://localhost:8080";
chai.use(chaiHttp);

describe.skip("c10k", () => {

  describe("10000 x GET /", () => {
    it("should allow 10k requests without killing the server", done => {
      const times: number = 10000;
      const requests = [];

      for (let i = 0; i < times; i++) {

        chai
          .request(SERVER_URL)
          .get("/")
          .end((err, res) => {
            if (err) done(err)

            // Check the response created by the server on the console
            console.log({ res, i });

            requests.push(i)

            res.should.have.status(200);
            res.should.be.html;
            res.text.should.include('Welcome to our server');

            if (requests.length === times) done();
          });
      }
    });
  });

})
