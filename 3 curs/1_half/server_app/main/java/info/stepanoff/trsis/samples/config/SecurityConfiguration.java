package info.stepanoff.trsis.samples.config;

import com.fasterxml.jackson.databind.ObjectMapper;
import info.stepanoff.trsis.samples.rest.ApiError;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.time.Instant;
import java.util.Map;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.http.HttpMethod;
import org.springframework.http.HttpStatus;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.util.matcher.AntPathRequestMatcher;

@Configuration
public class SecurityConfiguration {

    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http, ObjectMapper mapper) throws Exception {
        http.authorizeHttpRequests(auth -> auth
                .requestMatchers("/", "/login", "/css/**", "/js/**", "/webjars/**", "/swagger-ui/**",
                        "/swagger-ui.html", "/api-docs/**", "/v3/api-docs/**").permitAll()
                .requestMatchers(HttpMethod.GET, "/api/properties", "/api/properties/*").permitAll()
                .requestMatchers("/api/audit", "/api/audit/**").hasRole("EDITOR")
                .requestMatchers(HttpMethod.POST, "/api/properties").hasRole("EDITOR")
                .requestMatchers(HttpMethod.PUT, "/api/properties/*").hasRole("EDITOR")
                .requestMatchers(HttpMethod.DELETE, "/api/properties/*").hasRole("EDITOR")
                .anyRequest().authenticated())
            .formLogin(form -> form.loginPage("/login")
                    .usernameParameter("login")
                    .passwordParameter("pass")
                    .defaultSuccessUrl("/", false)
                    .permitAll())
            .logout(logout -> logout.logoutSuccessUrl("/?logout").permitAll())
            .exceptionHandling(errors -> errors
                    .defaultAuthenticationEntryPointFor(
                            (request, response, exception) -> writeError(response, mapper, HttpStatus.UNAUTHORIZED),
                            new AntPathRequestMatcher("/api/**"))
                    .accessDeniedHandler((request, response, exception) -> {
                        if (request.getRequestURI().startsWith("/api/")) {
                            writeError(response, mapper, HttpStatus.FORBIDDEN);
                        } else {
                            response.sendError(HttpStatus.FORBIDDEN.value());
                        }
                    }));
        return http.build();
    }

    private void writeError(HttpServletResponse response, ObjectMapper mapper, HttpStatus status) throws IOException {
        response.setStatus(status.value());
        response.setContentType("application/json;charset=UTF-8");
        mapper.writeValue(response.getWriter(), new ApiError(
                Instant.now(), status.value(), status.getReasonPhrase(), status.getReasonPhrase(), Map.of()));
    }
}
