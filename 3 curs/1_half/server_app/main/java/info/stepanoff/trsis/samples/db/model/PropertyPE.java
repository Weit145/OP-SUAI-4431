package info.stepanoff.trsis.samples.db.model;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.EnumType;
import jakarta.persistence.Enumerated;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import java.math.BigDecimal;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Entity
@Table(name = "PROPERTY")
@Getter
@Setter
@NoArgsConstructor
public class PropertyPE {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "PROPERTY_ID")
    private Long id;

    @Column(name = "ADDRESS", nullable = false)
    private String address;

    @Enumerated(EnumType.STRING)
    @Column(name = "PROPERTY_TYPE", nullable = false)
    private PropertyType type;

    @Column(name = "AREA", nullable = false, precision = 10, scale = 2)
    private BigDecimal area;

    @Column(name = "ROOMS", nullable = false)
    private Integer rooms;

    @Column(name = "MONTHLY_RENT", nullable = false, precision = 12, scale = 2)
    private BigDecimal monthlyRent;

    @Column(name = "AVAILABLE", nullable = false)
    private Boolean available;
}
