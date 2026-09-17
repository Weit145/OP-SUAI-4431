package info.stepanoff.trsis.samples;

import static org.hamcrest.Matchers.hasSize;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.delete;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.put;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.header;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;

@SpringBootTest
@AutoConfigureMockMvc
class PropertyRestControllerTest {

    @Autowired
    private MockMvc mockMvc;

    private static final String JSON = """
            {
              "address": "Санкт-Петербург, Тестовая улица, 1",
              "type": "APARTMENT",
              "area": 40.0,
              "rooms": 2,
              "monthlyRent": 50000,
              "available": true
            }
            """;

    @Test
    void supportsAllCrudOperations() throws Exception {
        mockMvc.perform(get("/api/properties"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$", hasSize(3)));

        mockMvc.perform(post("/api/properties").contentType(MediaType.APPLICATION_JSON).content(JSON))
                .andExpect(status().isCreated())
                .andExpect(header().exists("Location"))
                .andExpect(jsonPath("$.id").value(4));

        mockMvc.perform(put("/api/properties/4").contentType(MediaType.APPLICATION_JSON)
                        .content(JSON.replace("50000", "60000")))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.monthlyRent").value(60000));

        mockMvc.perform(delete("/api/properties/4"))
                .andExpect(status().isNoContent());

        mockMvc.perform(get("/api/properties/4"))
                .andExpect(status().isNotFound())
                .andExpect(jsonPath("$.status").value(404));
    }

    @Test
    void rejectsInvalidJsonData() throws Exception {
        mockMvc.perform(post("/api/properties").contentType(MediaType.APPLICATION_JSON)
                        .content(JSON.replace("\"rooms\": 2", "\"rooms\": 0")))
                .andExpect(status().isBadRequest())
                .andExpect(jsonPath("$.validationErrors.rooms").exists());
    }
}
