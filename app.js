// Sử dụng thư viện CryptoJS để mã hóa và giải mã tin nhắn
const CryptoJS = require("crypto-js");
// Sử dụng thư viện qrcode để tạo mã QR Code
const qrcode = require("qrcode");
const express = require("express");
const cors = require("cors");
const sharp = require("sharp");
const QRReader = require("qrcode-reader");
const Jimp = require("jimp");

const app = express();

app.use(express.json());
app.use(
  express.urlencoded({
    extended: true,
  })
);
app.use(cors());

// Sử dụng thuật toán hoán vị để trộn tin nhắn trước khi giấu vào trong QR Code
// Hàm suffle với cách hoán vị cụ thể
function shuffleString(str) {
  const arr = str.split("");
  const length = arr.length;
  for (let i = 0; i < length; i++) {
    const j = (i + 1) % length;
    [arr[i], arr[j]] = [arr[j], arr[i]];
  }
  return arr.join("");
}

// Hàm unshuffle tương ứng để giải trộn tin nhắn
function unshuffleString(str) {
  const arr = str.split("");
  const length = arr.length;
  for (let i = length - 1; i >= 0; i--) {
    const j = (i + 1) % length;
    [arr[i], arr[j]] = [arr[j], arr[i]];
  }
  return arr.join("");
}

// Mã hóa tin nhắn bằng AES
const message = "Hello, world!";
const key = "my-secret-key";

// Giải mã tin nhắn
const decryptedMessage = unshuffleString(
  CryptoJS.AES.decrypt(
    `U2FsdGVkX1+9gB4+8X8GpSYRxZ522tCe3xBv6R6TYew=`,
    key
  ).toString(CryptoJS.enc.Utf8)
);
console.log(decryptedMessage);

// Tạo mã QR Code từ tin nhắn đã được mã hóa và giải mã

//

app.post("/hide-message", (req, res) => {
  const { message } = req.body;
  const encryptedMessage = CryptoJS.AES.encrypt(
    shuffleString(message),
    key
  ).toString();
  qrcode.toDataURL(
    encryptedMessage,
    { errorCorrectionLevel: "H" },
    function (err, qrCodeDataURL) {
      if (err) throw err;
      // console.log(qrCodeDataURL);
      return res.status(200).json({qrCode: qrCodeDataURL, encryptMessage: encryptedMessage});
    }
  );
});

app.post("/decode-message", async (req, res) => {
  const base64String = req.body.qr_code;
  let base64Data = base64String.replace(/^data:image\/png;base64,/, "");
  let bufferData = Buffer.from(base64Data, "base64");

  // Tạo image buffer từ buffer dữ liệu
  Jimp.read(bufferData)
    .then((image) => {
      // Khởi tạo QR Reader
      let qr = new QRReader();

      // Đọc giá trị trong QR code
      qr.callback = (err, value) => {
        if (err) {
          console.log("Không thể đọc giá trị QR code:", err);
          return;
        }
        // console.log("Giá trị trong QR code:", value.result);
        const decryptedMessage = unshuffleString(
          CryptoJS.AES.decrypt(
            value.result,
            key
          ).toString(CryptoJS.enc.Utf8)
        );
        return res.status(200).json(decryptedMessage)

      };

      // Đọc QR code từ image buffer
      qr.decode(image.bitmap);
    })
    .catch((err) => {
      console.log("Không thể tạo image buffer từ chuỗi base64:", err);
    });
});

app.post("/decode-hide-message", (req, res)=> {
  const {message }= req.body
  const decryptedMessage = unshuffleString(
    CryptoJS.AES.decrypt(
      message,
      key
    ).toString(CryptoJS.enc.Utf8)
  );
  return res.status(200).json(decryptedMessage)
})

const PORT = 4000;
app.listen(PORT, () => {
  console.log(`Server API đang chạy trên cổng ${PORT}`);
});
