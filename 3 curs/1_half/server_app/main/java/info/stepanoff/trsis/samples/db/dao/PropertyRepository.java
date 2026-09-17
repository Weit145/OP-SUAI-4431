package info.stepanoff.trsis.samples.db.dao;

import info.stepanoff.trsis.samples.db.model.PropertyPE;
import org.springframework.data.jpa.repository.JpaRepository;

public interface PropertyRepository extends JpaRepository<PropertyPE, Long> {
}
