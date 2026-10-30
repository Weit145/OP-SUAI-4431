package info.stepanoff.trsis.samples;

import static org.awaitility.Awaitility.await;
import static org.junit.jupiter.api.Assertions.*;

import com.fasterxml.jackson.databind.ObjectMapper;
import info.stepanoff.trsis.samples.db.dao.AuditEventRepository;
import info.stepanoff.trsis.samples.replication.*;
import info.stepanoff.trsis.samples.rest.ResourceNotFoundException;
import info.stepanoff.trsis.samples.rest.model.PropertyDTO;
import info.stepanoff.trsis.samples.service.PropertyService;
import java.math.BigDecimal;
import java.time.Duration;
import java.time.Instant;
import java.util.UUID;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.builder.SpringApplicationBuilder;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.context.ConfigurableApplicationContext;
import org.springframework.kafka.core.KafkaTemplate;
import org.springframework.kafka.test.context.EmbeddedKafka;
import org.springframework.security.test.context.support.WithMockUser;
import org.springframework.test.annotation.DirtiesContext;

@SpringBootTest(properties = {
        "app.replication.enabled=true", "app.instance-id=test-first",
        "app.replication.topic=replication-test", "spring.datasource.url=jdbc:h2:mem:replica_first",
        "spring.jpa.show-sql=false", "logging.level.root=WARN"
})
@EmbeddedKafka(partitions = 1, topics = "replication-test", kraft = true,
        bootstrapServersProperty = "spring.kafka.bootstrap-servers")
@DirtiesContext
class ReplicationIntegrationTest {
    @Autowired private PropertyService first;
    @Autowired private ConfigurableApplicationContext context;
    @Autowired private KafkaTemplate<String, String> kafka;
    @Autowired private ObjectMapper mapper;

    private ConfigurableApplicationContext second() {
        return new SpringApplicationBuilder(Application.class).run(
                "--server.port=0", "--server.ssl.enabled=false",
                "--app.replication.enabled=true", "--app.instance-id=test-second",
                "--app.replication.topic=replication-test",
                "--spring.datasource.url=jdbc:h2:mem:replica_second",
                "--spring.kafka.bootstrap-servers=" + context.getEnvironment().getProperty("spring.kafka.bootstrap-servers"),
                "--spring.jpa.show-sql=false", "--logging.level.root=WARN");
    }

    @Test
    @WithMockUser(username = "guest", roles = "EDITOR")
    void replicatesBothDirectionsDeduplicatesAndReplaysAfterRestart() throws Exception {
        PropertyDTO property = new PropertyDTO();
        property.setAddress("Проверка Kafka, 7");
        property.setType(info.stepanoff.trsis.samples.db.model.PropertyType.APARTMENT);
        property.setArea(new BigDecimal("45.5"));
        property.setRooms(2);
        property.setMonthlyRent(new BigDecimal("50000"));
        property.setAvailable(true);
        Long deletedId;
        Long survivorId;
        try (var replica = second()) {
            var second = replica.getBean(PropertyService.class);
            deletedId = first.create(property).getId();
            await().ignoreException(ResourceNotFoundException.class).atMost(Duration.ofSeconds(30)).untilAsserted(() ->
                    assertEquals("guest", second.findById(deletedId).getOwnerLogin()));
            property.setMonthlyRent(new BigDecimal("65000"));
            second.update(deletedId, property);
            await().ignoreException(ResourceNotFoundException.class).atMost(Duration.ofSeconds(30)).untilAsserted(() ->
                    assertEquals(0, first.findById(deletedId).getMonthlyRent().compareTo(property.getMonthlyRent())));

            survivorId = second.create(property).getId();
            assertNotEquals(deletedId, survivorId);
            await().ignoreException(ResourceNotFoundException.class).atMost(Duration.ofSeconds(30))
                    .untilAsserted(() -> assertNotNull(first.findById(survivorId)));
            second.delete(deletedId);
            await().ignoreException(ResourceNotFoundException.class).atMost(Duration.ofSeconds(30)).untilAsserted(() -> {
                assertThrows(ResourceNotFoundException.class, () -> first.findById(deletedId));
                assertThrows(ResourceNotFoundException.class, () -> second.findById(deletedId));
                assertEquals(4, replica.getBean(AuditEventRepository.class).count());
            });

            // Send the very same event twice: exactly one change and audit entry per replica.
            PropertyChange event = new PropertyChange(UUID.randomUUID().toString(), "external",
                    "UPDATE", "guest", Instant.now(), second.findById(survivorId));
            String json = mapper.writeValueAsString(event);
            kafka.send("replication-test", survivorId.toString(), json).get();
            kafka.send("replication-test", survivorId.toString(), json).get();
            await().ignoreException(ResourceNotFoundException.class).atMost(Duration.ofSeconds(30)).untilAsserted(() -> {
                assertEquals(5, context.getBean(AuditEventRepository.class).count());
                assertEquals(5, replica.getBean(AuditEventRepository.class).count());
                assertEquals(0, replica.getBean(OutboxRepository.class).count());
            });
        }
        // A new H2 database must replay Kafka, even with the same instance identifier.
        try (var restarted = second()) {
            var second = restarted.getBean(PropertyService.class);
            await().ignoreException(ResourceNotFoundException.class).atMost(Duration.ofSeconds(30)).untilAsserted(() -> {
                assertNotNull(second.findById(survivorId));
                assertThrows(ResourceNotFoundException.class, () -> second.findById(deletedId));
                assertEquals(5, restarted.getBean(AuditEventRepository.class).count());
            });
        }
    }
}
