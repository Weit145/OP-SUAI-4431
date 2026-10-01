package info.stepanoff.trsis.samples;

import static org.hamcrest.Matchers.hasSize;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.delete;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.put;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.header;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;
import static org.springframework.security.test.web.servlet.request.SecurityMockMvcRequestPostProcessors.csrf;
import static org.springframework.security.test.web.servlet.response.SecurityMockMvcResultMatchers.authenticated;
import static org.springframework.security.test.web.servlet.response.SecurityMockMvcResultMatchers.unauthenticated;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.security.test.context.support.WithMockUser;

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
    @WithMockUser(username = "guest", roles = "EDITOR")
    void supportsAllCrudOperations() throws Exception {
        mockMvc.perform(get("/api/properties"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$", hasSize(3)));

        mockMvc.perform(post("/api/properties").with(csrf()).contentType(MediaType.APPLICATION_JSON).content(JSON))
                .andExpect(status().isCreated())
                .andExpect(header().exists("Location"))
                .andExpect(jsonPath("$.id").value(4));

        mockMvc.perform(put("/api/properties/4").with(csrf()).contentType(MediaType.APPLICATION_JSON)
                        .content(JSON.replace("50000", "60000")))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.monthlyRent").value(60000));

        mockMvc.perform(delete("/api/properties/4").with(csrf()))
                .andExpect(status().isNoContent());

        mockMvc.perform(get("/api/properties/4"))
                .andExpect(status().isNotFound())
                .andExpect(jsonPath("$.status").value(404));

        mockMvc.perform(get("/api/audit"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$[0].actor").value("guest"))
                .andExpect(jsonPath("$[0].action").value("DELETE"))
                .andExpect(jsonPath("$[1].action").value("UPDATE"))
                .andExpect(jsonPath("$[2].action").value("CREATE"));
    }

    @Test
    @WithMockUser(username = "guest", roles = "EDITOR")
    void rejectsInvalidJsonData() throws Exception {
        mockMvc.perform(post("/api/properties").with(csrf()).contentType(MediaType.APPLICATION_JSON)
                        .content(JSON.replace("\"rooms\": 2", "\"rooms\": 0")))
                .andExpect(status().isBadRequest())
                .andExpect(jsonPath("$.validationErrors.rooms").exists());
    }

    @Test
    void anonymousCanReadButCannotWrite() throws Exception {
        mockMvc.perform(get("/api/properties")).andExpect(status().isOk());
        mockMvc.perform(post("/api/properties").with(csrf()).contentType(MediaType.APPLICATION_JSON).content(JSON))
                .andExpect(status().isUnauthorized())
                .andExpect(jsonPath("$.status").value(401));
        mockMvc.perform(get("/api/audit")).andExpect(status().isUnauthorized());
    }

    @Test
    @WithMockUser(username = "guest", roles = "EDITOR")
    void writeNeedsCsrfToken() throws Exception {
        mockMvc.perform(post("/api/properties").contentType(MediaType.APPLICATION_JSON).content(JSON))
                .andExpect(status().isForbidden())
                .andExpect(jsonPath("$.status").value(403));
    }

    @Test
    void databaseUserCanLoginAndLogout() throws Exception {
        var login = mockMvc.perform(post("/login").with(csrf())
                        .param("login", "guest").param("pass", "hello"))
                .andExpect(status().is3xxRedirection())
                .andExpect(authenticated().withUsername("guest"))
                .andReturn();
        mockMvc.perform(post("/logout").with(csrf()).session((org.springframework.mock.web.MockHttpSession) login.getRequest().getSession()))
                .andExpect(status().is3xxRedirection())
                .andExpect(unauthenticated());
    }
}
