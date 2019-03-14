package io.viesure.iot.sign.controller;

import io.viesure.iot.sign.services.SignService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
public class SignController {

    @Autowired
    private SignService signService;

    @GetMapping(path = "/")
    public String index() {

        return "index";
    }

    @GetMapping(path = "/changeRGB")
    public ResponseEntity changeRGB(@RequestParam int red, @RequestParam int green, @RequestParam int blue) {

        signService.changeRGB(red, green, blue);
        return new ResponseEntity(HttpStatus.OK);
    }
}
