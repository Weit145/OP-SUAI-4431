package info.stepanoff.trsis.samples.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;

@Controller
public class PagesController {
    @GetMapping("/")
    public String properties() {
        return "properties";
    }

    @GetMapping("/login")
    public String login() {
        return "login";
    }
}
