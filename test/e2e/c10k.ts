import chai from "chai";
import chaiHttp from "chai-http";
import http from 'http';

chai.should();
const SERVER_URL = process.env.APP_URL || "http://localhost:8080";
const SERVER_HOST = SERVER_URL ? null : "localhost";
chai.use(chaiHttp);

describe.skip("c10k", () => {

  describe("1000 x GET /", () => {
    it("should allow 1k requests without killing the server", done => {
      const times = 1000;
      const results: (number | undefined)[] = [];
      let j = 0;

      for (let i = 0; i < times; i++) {
        http.request({
          host: SERVER_HOST,
          port: 8080,
          path: '/'
        }, function (res) {
          results.push(res.statusCode);
          j++;

          if (j == i) { // last request
            const filterred = results.filter((result) => !!result);

            if (filterred.length === times) {
              done()
            } else {
              done('failed');
            }
          }
        }).end();
      }

    });
  });

  describe("10000 x GET /", () => {
    it("should allow 10k requests without killing the server", done => {
      const times = 10000;
      const results: (number | undefined)[] = [];
      let j = 0;

      for (let i = 0; i < times; i++) {
        http.request({
          host: SERVER_HOST,
          port: 8080,
          path: '/'
        }, function (res) {
          results.push(res.statusCode);
          j++;

          if (j == i) { // last request
            const filterred = results.filter((result) => !!result);

            if (filterred.length === times) {
              done()
            } else {
              done('failed');
            }
          }
        }).end();
      }

    });
  });

})
